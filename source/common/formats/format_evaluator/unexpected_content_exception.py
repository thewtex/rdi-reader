class UnexpectedContent(Exception):
    def __init__(self, message="Unexpected content for a .rdi file."):
        self.message = message
    def __str__(self):
        return repr(self.message)

