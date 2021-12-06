import os
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# https://openlibrary.org/developers/api
open_library_url = "http://openlibrary.org/"

@app.route("/", methods=["GET", "POST"])
def index():
    # Query API for book for a random book with certain parameters
    if request.method == "POST":
        if request.form.get("genre"):
            query_url = os.path.join(open_library_url, "subjects", request.form.get("genre") + ".json?published_in=2021&limit=1")
            print(query_url)
            return render_template("index.html")
            
    else:
        return render_template("index.html")

@app.route("/book", methods=["GET", "POST"])
def book():
    # Result of book
    return apology("TODO")

@app.route("/random", methods=["GET", "POST"])
def random():
    # Query API for book and generate random book
    return apology("TODO")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
