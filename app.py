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



@app.route("/hash", methods=["GET","POST"])
def hash():
  if request.method=="POST":
    valor = request.form.get('exampleFormControlTextarea1')
    if(valor.find('*')>0 and valor.find('=')<0 and valor.find("between")<0 and valor.find("and")<0 and valor.find("delete")<0):
      process = subprocess.call(["./ehash.out", "print"], shell=False)
      with open("CSV/ehash.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)

    if(valor.find("=")>0 and valor.find("between")<0 and valor.find("delete")<0):
      equal_value = valor.split("=")[1][:-1]
      process = subprocess.call(["./ehash.out", "search", equal_value])
      with open("CSV/ehash.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)

    #Between
    if(valor.find("between")>0 and valor.find("and")>0 and valor.find("delete")<0) :
      #print(first,last)
      first = valor.split(" ")[-3]
      last = valor.split(" ")[-1][:-1]
      juntar=first+","+last
      print(juntar)
      process = subprocess.call(["./ehash.out", "rangeSearch", juntar])
      with open("CSV/ehash.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file) 

    #Delete
    if(valor.find("delete")>=0):
      first = valor.split("=")[1][:-1]
      print(first)
      process = subprocess.call(["./ehash.out", "remove", first])
      with open("CSV/ehash.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)

    # Insertar 
    if(valor.find("insert")>=0):
      #print("ENTROOOOOO")
      first = valor.split("(")[1][:-2]
      #print(first)
      process = subprocess.call(["./ehash.out", "add", first])
      with open("CSV/ehash.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)
    return render_template("table.html", csv=file)

  else:
    return render_template("hash.html")




@app.route("/sequential", methods=["GET","POST"])
def sequential():

  if request.method=="POST":

    valor = request.form.get('exampleFormControlTextarea1')
    if(valor.find('*')>0 and valor.find('=')<0 and valor.find("between")<0 and valor.find("and")<0 and valor.find("delete")<0):

      process = subprocess.call(["./seqf.out", "-p"], shell=False)
      with open("CSV/sequential.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)

    if(valor.find("=")>0 and valor.find("between")<0 and valor.find("delete")<0):
      equal_value = valor.split("=")[1][:-1]
      print("EL VALOR ES",valor)
      print("EL EQUAL ES",equal_value)
      process = subprocess.call(["./seqf.out", "-s", equal_value])
      with open("CSV/sequential.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)


    if(valor.find("between")>0 and valor.find("and")>0 and valor.find("delete")<0) :
      first = valor.split(" ")[-3]
      last = valor.split(" ")[-1][:-1]
      print(first,last)
      process = subprocess.call(["./seqf.out", "-s", first , last])
      with open("CSV/sequential.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)   


    if(valor.find("delete")>=0):
      first = valor.split("=")[1][:-1]
      print(first)
      process = subprocess.call(["./seqf.out", "-d", first])
      with open("CSV/sequential.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)

    if(valor.find("insert")>=0):

      first = valor.split("(")[1][:-2]
      print(first)
      process = subprocess.call(["./seqf.out", "-i", first])
      with open("CSV/sequential.csv", encoding='utf-8', errors='ignore') as file:
        return render_template("table.html", csv=file)
    return render_template("table.html", csv=file)

  else:
    return render_template("sequential.html")



# (C) START
if __name__ == "__main__":
  app.run(HOST_NAME, HOST_PORT, debug=True)