import flask
from flask import request, jsonify
from flask_cors import CORS, cross_origin
import base64
import os
import sys
import json
import time


app = flask.Flask(__name__)
app.config["DEBUG"] = True
cors = CORS(app)


@app.route('/', methods=['POST'])
@cross_origin()
def home():
    # print(jsonify(request.json))
    if int(request.form['type']) == 1:
        filename = request.form['data[filename]']
        with open("./vod_nodejs/serverside/video/" + filename, "wb") as video_file:
            video_file.write(base64.b64decode(request.form['data[file]'][22:]))
        print("Video saved")
    return "200"


app.run(host='127.0.0.1', port=5000)
