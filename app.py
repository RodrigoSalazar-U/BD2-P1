# (A) INIT
# (A1) LOAD MODULES
from asyncio import subprocess
from flask import Flask, render_template, request, make_response
import subprocess
import webbrowser


# (A2) FLASK SETTINGS + INIT
HOST_NAME = "localhost"
HOST_PORT = 8080
app = Flask(__name__)
# app.debug = True
 
# (B) DEMO - READ CSV & GENERATE HTML TABLE
@app.route("/", methods=["GET","POST"])
def index():
  if request.method=="POST":

    if(button1 or button2) == None:
      return render_template("home.html")

    print(request.form.get('exampleFormControlTextarea1'))
    webbrowser.open('http://localhost:8080/hash')
    return render_template("home.html")

  else:
    return render_template("home.html")

@app.route("/alumnos")
def alumnos():
  process = subprocess.call(["./a.out", "-p"], shell=False)
  #print("Acabo")
  with open("InputFiles/dataset.csv", encoding='utf-8', errors='ignore') as file:
    return render_template("table.html", csv=file)



# (C) START
if __name__ == "__main__":
  app.run(HOST_NAME, HOST_PORT, debug=True)