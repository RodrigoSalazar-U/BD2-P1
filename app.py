# (A) INIT
# (A1) LOAD MODULES
from asyncio import subprocess
from flask import Flask, render_template, request, make_response
import subprocess


# (A2) FLASK SETTINGS + INIT
HOST_NAME = "localhost"
HOST_PORT = 8080
app = Flask(__name__)
# app.debug = True
 
# (B) DEMO - READ CSV & GENERATE HTML TABLE
@app.route("/")
def index():
  #process = subprocess.call("./a.out", shell=False)
  #print("Acabo")
  #with open("example.csv") as file:
  return render_template("home.html")

@app.route("/alumnos")
def alumnos():
  process = subprocess.call("./a.out", shell=False)
  print("Acabo")
  with open("example.csv") as file:
    return render_template("table.html", csv=file)



# (C) START
if __name__ == "__main__":
  app.run(HOST_NAME, HOST_PORT, debug=True)