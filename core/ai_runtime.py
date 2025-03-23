from sentence_transformers import SentenceTransformer
import numpy as np
from core.values import String, List, Number
from utils.errors import RTError

class EmbeddingManager:
    def __init__(self):
        self.default_model = SentenceTransformer('all-MiniLM-L6-v2')
        self.models = {
            'default': self.default_model
        }
    
    def embed(self, text, model_name='default'):
        """Generate embeddings for the given text."""
        model = self.models.get(model_name, self.default_model)
        embedding = model.encode(text)
        return List([Number(float(x)) for x in embedding])

    def cosine_similarity(self, vec1, vec2):
        """Calculate cosine similarity between two vectors."""
        if not isinstance(vec1, List) or not isinstance(vec2, List):
            raise ValueError("Both arguments must be lists")
        
        v1 = np.array([x.value for x in vec1.elements])
        v2 = np.array([x.value for x in vec2.elements])
        
        similarity = np.dot(v1, v2) / (np.linalg.norm(v1) * np.linalg.norm(v2))
        return Number(float(similarity))

class AIManager:
    def __init__(self):
        self.models = {}
        self.embedding_manager = EmbeddingManager()
    
    def register_model(self, name, model):
        """Register a new model."""
        self.models[name] = model
    
    def call_model(self, model_name, args, pos_start=None, pos_end=None, context=None):
        """Call an AI model with the given arguments."""
        if model_name not in self.models:
            return None, RTError(
                pos_start, pos_end,
                f"Model '{model_name}' not found",
                context
            )
        
        try:
            result = self.models[model_name](*args)
            if isinstance(result, str):
                return String(result), None
            elif isinstance(result, (int, float)):
                return Number(result), None
            elif isinstance(result, list):
                return List([Number(x) if isinstance(x, (int, float)) else String(str(x)) for x in result]), None
            else:
                return String(str(result)), None
        except Exception as e:
            return None, RTError(
                pos_start, pos_end,
                f"Error calling model '{model_name}': {str(e)}",
                context
            ) 