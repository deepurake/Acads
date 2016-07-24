from flask import Flask
from get_list import get_json
from flask.ext.cors import CORS
app = Flask(__name__)
CORS(app)

@app.route("/<address>", methods=['GET', 'OPTIONS'])
def get_list(address):
    return get_json(address)


if __name__ == "__main__":
    app.run(threaded=True)