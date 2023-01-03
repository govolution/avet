"""
Implementation of Genetic Optimization Algorithm from
Functionality-Preserving Black-Box Optimization of Adversarial Windows Malware
by Demetrio et al.
"""

import argparse
import copy
import os
import random
import sys

import lief
import magic
import numpy as np
from malconv.malconv import CClassifierEnd2EndMalware, End2EndModel, MalConv
from practical_manipulations.section_injection import section_injection_on_bytes
from practical_manipulations.extend import extend_on_bytes
from practical_manipulations.padding import padding_on_bytes
from practical_manipulations.full_dos import full_dos_on_bytes
from practical_manipulations.partial_dos import partial_dos_on_bytes
from practical_manipulations.shift import shift_on_bytes


def create_population_from_benign_sections(goodware_folder="input/goodware_samples/", population_size=10):
    """Return a randomly chosen list of benign sections."""

    section_population = []

    goodware_list = os.listdir(goodware_folder)

    random_sections = random.sample(goodware_list, population_size)

    for fn in random_sections:
        file_path = os.path.join(goodware_folder, fn)
        if "PE" not in magic.from_file(file_path):
            continue

        exe_object: lief.PE.Binary = lief.parse(file_path)

        for section in exe_object.sections:
            if section.name == ".data":
                #section_population.append((bytearray(section.content), fn))
                section_population.append(bytearray(section.content))

    return section_population


def classifier_score_from_file(classifier: CClassifierEnd2EndMalware, path_to_file):
    """Return malware score from file path"""

    if "PE32" not in magic.from_file(path_to_file):
        print("Not a PE32 file")
        return

    with open(path_to_file, "rb") as file_handle:
        code = file_handle.read()
        x = End2EndModel.bytes_to_numpy(
            code, classifier.get_input_max_length(), 256, False
        )
        _, confidence = classifier.predict(x, True)

        return confidence[0, 1].item()


def classifier_score_from_bytes(classifier: CClassifierEnd2EndMalware, adv_bytes: bytearray):
    """Return malware score from bytes"""

    x = End2EndModel.bytes_to_numpy(
        adv_bytes, classifier.get_input_max_length(), 256, False
    )
    _, confidence = classifier.predict(x, True)

    return confidence[0, 1].item()


def size_of_payload(candidate):
    """Return size of the payload"""
    size = 0

    for s, t in zip(*candidate):
        size += int(round(len(s) * t))
    return size


def objective_function(classifier: CClassifierEnd2EndMalware, adv_bytes: bytearray, penalty_regularizer, candidate):
    """Return score from objective function"""

    score = classifier_score_from_bytes(classifier, adv_bytes)

    penalty_term = penalty_regularizer * size_of_payload(candidate)

    return score + penalty_term


def apply_practical_manipulation(adv_bytes: bytearray, section_population, vector_t, practical_manipulation):
    """Apply the chosen practical manipulation and Returns new bytes with the practical manipulation applied"""

    return practical_manipulation(adv_bytes, section_population, vector_t)


def crossover(old_gen: list, population_size):
    """Return new list of solution candidates through crossover"""

    offsprings = []
    while len(offsprings) != population_size:
        # pick two parents randomly
        p1 = random.choice(old_gen)
        p2 = random.choice(old_gen)

        j = random.randrange(len(p1[1]))
        child = p1[0][:j] + p2[0][j:]
        offsprings.append((child, p1[1]))

    return offsprings


def mutation(candidate, mutation_probability):
    """
    Applies mutation with the specified probability.
    """

    for i in range(len(candidate[1])):
        if random.random() < mutation_probability:
            candidate[1][i] = random.random()

    return candidate


def main():
    parser = argparse.ArgumentParser(
        prog='Genetic Optimizer',
        description="Genetic Optimizer for Injected Bytes"
    )

    parser.add_argument('filename')
    parser.add_argument('-p', '--population_size', type=int, default=50)
    parser.add_argument('-e', '--elitsm', type=int, default=10)
    parser.add_argument('-i', '--iteration', type=int, default=10)
    parser.add_argument('-pr', '--penalty_regularizer',
                        type=float, default=1e-6)
    parser.add_argument('-ss', '--section_size', type=int, default=10)
    parser.add_argument('-pm', '--practical_manipulation',
                        default="section_injection")

    args = parser.parse_args()

    net = CClassifierEnd2EndMalware(MalConv())
    net.load_pretrained_model()

    file_name = args.filename
    pop_size = args.population_size
    elitism = args.elitsm
    max_iteration = args.iteration
    penalty_regularizer = args.penalty_regularizer
    section_size = args.section_size
    pm = section_injection_on_bytes

    match args.practical_manipulation:
        case "section_injection":
            pm = section_injection_on_bytes
        case "padding":
            pm = padding_on_bytes
        case "full_dos":
            pm = full_dos_on_bytes
        case "partial_dos":
            pm = partial_dos_on_bytes
        case "extend":
            pm = extend_on_bytes
        case "shift":
            pm = shift_on_bytes
        case _:
            print("Practical Manipulation does not exist")
            sys.exit(1)

    print(f"Technique: {args.practical_manipulation}")

    adv_bytes = bytearray()
    with open(file_name, 'rb') as f:
        adv_bytes = bytearray(f.read())

    print("--- Initial Score ---")
    print(classifier_score_from_bytes(net, adv_bytes))

    print("Extracting Sections...")
    sec_pop = create_population_from_benign_sections(
        population_size=section_size)

    print(f"Using Genetic Optimizer with {pop_size} candidates")

    candidates = []
    for _ in range(pop_size):
        candidates.append(
            (sec_pop, np.random.random_sample(size=len(sec_pop))))

    # track minimum
    min_counter = 0
    previous_score = 0

    for i in range(max_iteration):

        # rank canditates
        rank = []
        for candidate in candidates:
            pm_adv_bytes = apply_practical_manipulation(
                adv_bytes, candidate[0], candidate[1], pm)

            rank.append(
                (objective_function(net, pm_adv_bytes, penalty_regularizer, candidate), candidate, classifier_score_from_bytes(net, pm_adv_bytes), pm_adv_bytes))

        rank.sort(key=lambda tup: tup[0])

        print(f"=== Gen {i} best solution_candidates ===")
        print(
            f"OF: {rank[0][0]}, MS: {rank[0][2]}, PS: {size_of_payload(rank[0][1])}")

        # check if best candidate did not change for 5 generation.
        # Proceed if still not evaded.
        if i == max_iteration-1 or (min_counter >= 3 and rank[0][2] < 0.5):
            with open(f"{file_name[:-4]}_adv.exe", 'wb') as w:
                w.write(rank[0][3])
                break

        if previous_score == rank[0][0]:
            min_counter += 1
        else:
            min_counter = 0

        previous_score = rank[0][0]

        candidates = []

        # selection
        selection = copy.deepcopy(rank[:elitism])
        without_rank_selection = []
        for w in selection:
            without_rank_selection.append(w[1])

        # elitism
        candidates += copy.deepcopy(without_rank_selection)

        # crossover
        offsprings = crossover(copy.deepcopy(
            without_rank_selection), pop_size-elitism)

        # mutation
        mut = copy.deepcopy(offsprings)
        for m in mut:
            mutation(m, 0.5)

        # new generation
        candidates += copy.deepcopy(mut)


if __name__ == "__main__":
    main()
