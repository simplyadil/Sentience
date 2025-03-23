class EmbedNode:
    def __init__(self, text_node, model_node=None):
        self.text_node = text_node
        self.model_node = model_node
        self.pos_start = text_node.pos_start
        self.pos_end = model_node.pos_end if model_node else text_node.pos_end

    def __repr__(self):
        if self.model_node:
            return f'(EMBED {self.text_node} WITH {self.model_node})'
        return f'(EMBED {self.text_node})'

class AICallNode:
    def __init__(self, model_name, args):
        self.model_name = model_name
        self.args = args
        self.pos_start = model_name.pos_start
        self.pos_end = (args[-1].pos_end if args else model_name.pos_end)

    def __repr__(self):
        return f'(AI {self.model_name} {self.args})'

class PipeNode:
    def __init__(self, left_node, right_node):
        self.left_node = left_node
        self.right_node = right_node
        self.pos_start = left_node.pos_start
        self.pos_end = right_node.pos_end

    def __repr__(self):
        return f'({self.left_node} PIPE {self.right_node})' 