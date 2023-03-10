"""
Malware Detection by Eating a Whole EXE
Edward Raff, Jon Barker, Jared Sylvester, Robert Brandon, Bryan Catanzaro, Charles Nicholas
https://arxiv.org/abs/1710.09435

Malconv implementation extracted from https://github.com/pralab/secml_malware.
Pre-trained MalConv model trained by EndGame
"""

from abc import abstractmethod
import os

import numpy as np
import torch
import torch.nn.functional as F
from torch.nn import Module
import torch.nn as nn
import torchvision.transforms as transforms

from secml.array import CArray
from secml.ml.classifiers.pytorch.c_classifier_pytorch import CClassifierPyTorch
from secml.settings import SECML_PYTORCH_USE_CUDA

use_cuda = torch.cuda.is_available() and SECML_PYTORCH_USE_CUDA


class End2EndModel(Module):

    def __init__(self, embedding_size: int, max_input_size: int, embedding_value: int, shift_values: bool):
        """
        Basic end to end model wrapper

        Parameters
        ----------
        embedding_size : int
                the size of the embedding space
        max_input_size : int
                the input window size
        embedding_value : int
                the value used as padding
        shift_values : bool
                True if values are shifted by one
        """
        super(End2EndModel, self).__init__()
        self.embedding_size = embedding_size
        self.max_input_size = max_input_size
        self.embedding_value = embedding_value
        self.shift_values = shift_values

    @classmethod
    def path_to_exe_vector(cls, path: str, max_length: int, padding_value: int, shift_values: bool) -> np.ndarray:
        """
        Creates a numpy array from the bytes contained in file

        Parameters
        ----------
        path : str
                the path of the file
        max_length : int
                the max input size of the network
        padding_value : int
                the value used as padding
        shift_values : bool
                True if values are shifted by one

        Returns
        -------
        numpy array
                the sample as numpy array, cropped or padded
        """
        exe = cls.load_sample_from_file(path, max_length, has_shape=True, padding_value=padding_value,
                                        shift_values=shift_values)
        return exe.reshape(max_length)

    @classmethod
    @abstractmethod
    def bytes_to_numpy(cls, bytez: bytes, max_length: int, padding_value: int, shift_values: bool) -> np.ndarray:
        """
        It creates a numpy array from bare bytes. The vector is max_length long.

        Parameters
        ----------
        bytez : bytes
                byte string containing the sample
        max_length : int
                the max input size of the network
        padding_value : int
                the value used as padding
        shift_values : bool
                True if values are shifted by one

        Returns
        -------
        numpy array
                the sample as numpy array, cropped or padded

        """
        b = np.ones((max_length,), dtype=np.uint16) * padding_value
        bytez = np.frombuffer(bytez[:max_length], dtype=np.uint8)
        bytez = bytez.astype(np.uint16) + shift_values
        b[: len(bytez)] = bytez
        return np.array(b, dtype=float)

    @classmethod
    @abstractmethod
    def list_to_numpy(cls, x, max_length, padding_value, shift_values):
        """
        It creates a numpy array from bare bytes. The vector is max_length long.
        """
        b = np.ones((max_length,), dtype=np.uint16) * padding_value
        bytez = np.array(x[:max_length], dtype=np.uint8)
        bytez = bytez.astype(np.uint16) + shift_values
        b[: len(bytez)] = bytez
        return np.array(b, dtype=float)

    @classmethod
    @abstractmethod
    def load_sample_from_file(cls, path, max_length, has_shape, padding_value, shift_values):
        """
        It creates a numpy array containing a sample. The vector is max_length long.
        If shape is true, then the path is supposed to be a (1,1) matrix.
        Hence, item() is called.
        """
        file_path = path.item() if has_shape else path
        with open(file_path, "rb") as malware:
            code = cls.bytes_to_numpy(
                malware.read(), max_length, padding_value, shift_values)
        return code

    @abstractmethod
    def embed(self, input_x, transpose=True):
        """
        It embeds an input vector into MalConv embedded representation.
        """
        pass

    # @abstractmethod
    def compute_embedding_gradient(self, numpy_x: np.ndarray) -> torch.Tensor:
        """
        It computes the gradient w.r.t. the embedding layer.

        Parameters
        ----------
        numpy_x : numpy array
                the numpy array containing a sample
        Returns
        -------
        torch.Tensor
                the gradient w.r.t. the embedding layer
        """
        emb_x = self.embed(numpy_x)
        y = self.embedd_and_forward(emb_x)
        g = torch.autograd.grad(y, emb_x)[0]
        g = torch.transpose(g, 1, 2)[0]
        return g

    @abstractmethod
    def embedd_and_forward(self, x: torch.Tensor) -> torch.Tensor:
        """
        Compute the embedding for sample x and returns the prediction.

        Parameters
        ----------
        x : torch.Tensor
                the sample as torch tensor
        Returns
        -------
        torch.Tensor
                the result of the forward pass
        """
        pass

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        """
        Forward pass.

        Parameters
        ----------
        x : torch.Tensor
                the sample to test
        Returns
        -------
        torch.Tensor
                the result of the forward pass
        """
        embedding_1 = self.embed(x.contiguous())
        output = self.embedd_and_forward(embedding_1)
        return output

    def load_simplified_model(self, path: str):
        """
        Load the model weights.

        Parameters
        ----------
        path : str
                the path to the model
        """
        if not os.path.isfile(path):
            print(f"{path} path not pointing to regular file!")
        f = torch.load(path) if use_cuda else torch.load(
            path, map_location="cpu")
        self.load_state_dict(f)
        self.eval()


class CClassifierEnd2EndMalware(CClassifierPyTorch):

    def __init__(
        self,
        model: End2EndModel,
        epochs=100,
        batch_size=256,
        train_transform=None,
        preprocess=None,
        softmax_outputs=False,
        random_state=None,
        plus_version=False,
        input_shape=(1, 2 ** 20),
        verbose=0,
    ):
        super(CClassifierEnd2EndMalware, self).__init__(
            model,
            loss="binary_crossentropy",
            epochs=epochs,
            batch_size=batch_size,
            preprocess=preprocess,
            input_shape=(1, model.max_input_size),
            softmax_outputs=softmax_outputs,
            random_state=random_state,
        )
        self.plus_version = plus_version
        self.verbose = verbose
        self.train_transform = (
            train_transform
            if train_transform is not None
            else transforms.Lambda(lambda p: p.reshape(input_shape[1]))
        )

    def gradient(self, x, w=None):
        """Compute gradient at x by doing a forward and a backward pass.

        The gradient is pre-multiplied by w.

        """
        return self._gradient_f(x)

    def gradient_f_x(self, x, **kwargs):
        """Returns the gradient of the function on point x.

        Arguments:
                x {CArray} -- The point

        Raises:
                NotImplementedError: Model do not support gradient

        Returns:
                CArray -- the gradient computed on x
        """
        if self.preprocess is not None:
            # Normalize data before compute the classifier gradient
            x_pre = self.preprocess.normalize(x)
        else:  # Data will not be preprocessed
            x_pre = x
        try:  # Get the derivative of decision_function
            grad_f = self._gradient_f(x_pre, **kwargs)
        except NotImplementedError:
            raise NotImplementedError(
                "{:} does not implement `gradient_f_x`".format(
                    self.__class__.__name__)
            )
        return grad_f

    def _gradient_f(self, x, y=None, w=None, layer=None, sum_embedding=True):
        penalty_term = torch.zeros(1)
        penalty_term.requires_grad_()
        gradient = self.compute_embedding_gradient(x.tondarray(), penalty_term)
        if sum_embedding:
            gradient = torch.mean(gradient, dim=1)
        if gradient.is_cuda:
            gradient = gradient.cpu()
        return CArray(gradient)

    def load_pretrained_model(self):
        """
        Load pretrained model weights

        Parameters
        ----------
        path : str, optional, default None
                The path of the model, default is None, and it will load the internal default one
        """

        self._model.load_simplified_model(
            "source/implementations/gen_adversarial_exe/malconv/pretrained_malconv.pth")
        self._classes = np.array([0, 1])
        self._n_features = 2 ** 20

    def get_embedding_size(self):
        """
        Get the embedding space dimensionality

        Returns
        -------
        int
                the dimensionality of the embedding space
        """
        return self._model.embedding_size

    def get_input_max_length(self):
        """
        Get the input window length

        Returns
        -------
        int
                the window input length
        """
        return self._model.max_input_size

    def get_embedding_value(self):
        """
        Get the value used as padding

        Returns
        -------
        int
                a value that is used for padding the sample
        """
        return self._model.embedding_value

    def get_is_shifting_values(self):
        """
        Get if the model shifts the values by one

        Returns
        -------
        bool
                return if the values are shifted by one
        """
        return self._model.shift_values

    def embed(self, x: CArray, transpose: bool = True):
        """
        Embed the sample inside the embedding space

        Parameters
        ----------
        x : CArray
                the sample to embed
        transpose : bool, optional, default True
                set True to return the transposed feature space vector
        Returns
        -------
        torch.Tensor
                the embedded vector
        """
        return self._model.embed(x, transpose=transpose)

    def compute_embedding_gradient(self, x: CArray, penalty_term: torch.Tensor):
        """Compute the gradient w.r.t. embedding layer.

        Parameters
        ----------
        x : CArray
                point where gradient will be computed
        penalty_term : float
                the penalty term

        Returns
        ----------
        CArray
                the gradient w.r.t. the embedding
        """
        data = x
        if isinstance(x, CArray):
            data = x.tondarray()
        emb_x = self.embed(data)
        y = self.model.embedd_and_forward(emb_x)
        output = y + penalty_term
        if use_cuda:
            output = output.cuda()
        g = torch.autograd.grad(output, emb_x)[0]
        g = torch.transpose(g, 1, 2)[0]
        return g

    def embedding_predict(self, x):
        """
        Embed the sample and produce prediction.

        Parameters
        ----------
        x : CArray
                the input sample

        Returns
        -------
        float
                the malware score
        """
        return self._model.embedd_and_forward(x)

    def _forward(self, x):
        x = x.atleast_2d()
        scores = super(CClassifierEnd2EndMalware, self)._forward(x)
        confidence = []
        for i in range(x.shape[0]):
            confidence.append([1 - scores[i, 1].item(), scores[i, 1].item()])
        confidence = CArray(confidence)
        return confidence


class MalConv(End2EndModel):
    """
    Architecture implementation.
    """

    def __init__(self, pretrained_path=None, embedding_size=8, max_input_size=2 ** 20):
        super(MalConv, self).__init__(
            embedding_size, max_input_size, 256, False)
        self.embedding_1 = nn.Embedding(
            num_embeddings=257, embedding_dim=embedding_size)
        self.conv1d_1 = nn.Conv1d(in_channels=embedding_size, out_channels=128, kernel_size=(500,), stride=(500,),
                                  groups=1, bias=True)
        self.conv1d_2 = nn.Conv1d(in_channels=embedding_size, out_channels=128, kernel_size=(500,), stride=(500,),
                                  groups=1, bias=True)
        self.dense_1 = nn.Linear(in_features=128, out_features=128, bias=True)
        self.dense_2 = nn.Linear(in_features=128, out_features=1, bias=True)
        if pretrained_path is not None:
            self.load_simplified_model(pretrained_path)
        if use_cuda:
            self.cuda()

    def embed(self, input_x, transpose=True):
        if isinstance(input_x, torch.Tensor):
            x = input_x.clone().detach().requires_grad_(True).type(torch.LongTensor)
        else:
            x = torch.from_numpy(input_x).type(torch.LongTensor)
        x = x.squeeze(dim=1)
        if use_cuda:
            x = x.cuda()
        emb_x = self.embedding_1(x)
        if transpose:
            emb_x = torch.transpose(emb_x, 1, 2)
        return emb_x

    def embedd_and_forward(self, x):
        conv1d_1 = self.conv1d_1(x)
        conv1d_2 = self.conv1d_2(x)
        conv1d_1_activation = torch.relu(conv1d_1)
        conv1d_2_activation = torch.sigmoid(conv1d_2)
        multiply_1 = conv1d_1_activation * conv1d_2_activation
        global_max_pooling1d_1 = F.max_pool1d(
            input=multiply_1, kernel_size=multiply_1.size()[2:])
        global_max_pooling1d_1_flatten = global_max_pooling1d_1.view(
            global_max_pooling1d_1.size(0), -1)
        dense_1 = self.dense_1(global_max_pooling1d_1_flatten)
        dense_1_activation = torch.relu(dense_1)
        dense_2 = self.dense_2(dense_1_activation)
        dense_2_activation = torch.sigmoid(dense_2)
        return dense_2_activation
