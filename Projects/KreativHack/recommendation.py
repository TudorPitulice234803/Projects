from typing import Optional, Iterable
import numpy as np
import pandas as pd
from tqdm.auto import tqdm
import wikipedia
from sentence_transformers import SentenceTransformer


def inference(interests: list[str]):
    """
    Given a list of user interests, recommend relevant Wikipedia articles.

    Args:
        interests (list[str]): List of user interests.
    Returns:
        list[dict]: List of recommended monuments/museums based on word2vec
                    similarity.
    """

    MAX_ARTICLE_CHARS = 4000
    BATCH_SIZE = 32
    MODEL = SentenceTransformer("sentence-transformers/all-MiniLM-L6-v2")

    
