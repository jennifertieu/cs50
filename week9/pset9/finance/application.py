import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # For the current user, show which stock the user owns, number of shares owned,
    # the current price of each stock, and the total value of each holding
    # (i.e., shares times price). Also display the user’s current cash balance along
    # with a grand total (i.e., stocks’ total value plus cash).
    portfolio = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])
    balance = [row['cash'] for row in db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])][0]
    stock_total = 0
    # Create a dictionary of the current price of the stock
    stock_price = {}
    # Create a dictionary of the total value of each holding
    stock_holding = {}
    if len(portfolio) > 0:
        for stock in portfolio:
            # Get the current stock price
            current_price = lookup(stock['symbol'])["price"]
            stock_price[stock['symbol']] = current_price
            # Calculate the total value of each holding
            total_value = stock['shares'] * current_price
            stock_holding[stock['symbol']] = total_value
            stock_total += total_value
    else:
        # Set default values
        portfolio = [{'symbol': None, 'shares': 0}]
        stock_price[None] = 0
        stock_holding[None] = 0

    grand_total = stock_total + balance

    return render_template("index.html", portfolio=portfolio, balance=balance, stock_price=stock_price, stock_holding=stock_holding, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Require that a user input a stock's symbol
        if lookup(request.form.get("symbol")) is None:
            # Render apology if the input is blank or the symbol does not exist per lookup return value
            return apology("invalid stock symbol", 400)
        elif request.form.get("shares") is None:
            return apology("must input number of shares", 400)
        elif not request.form.get("shares").isdigit():
            return apology("must be a positive integer", 400)
        # Require that a user's input a number of shares
        elif int(request.form.get("shares")) < 0:
            # Return apology if not a positive integer
            return apology("must be a positive integer", 400)

        # Call lookup to look up a stock's current price
        buy_price = lookup(request.form.get("symbol"))["price"] * int(request.form.get("shares"))
        # Select how much cash the user currently has in user
        row = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        if buy_price > row[0]['cash']:
            # Return an apology without completing a purchase, if the user cannot afford the number of shares at the current price
            return apology("insufficient funds", 400)
        else:
            # Add purchase to table
            db.execute("INSERT INTO transactions (user_id, symbol, price, shares, type) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], request.form.get("symbol").upper(), buy_price, request.form.get("shares"), "BUY")
            # Subtract purchase from user's cash
            db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", buy_price, session["user_id"])
            # Redirect the user back to the index page
            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # For each row, make clear whether a stock was bought or sold and include the stock’s symbol,
    # the (purchase or sale) price, the number of shares bought or sold, and the date and time at which the transaction occurred.
    transactions = db.execute("SELECT type, symbol, price, shares, date FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)
        else:
            results = lookup(request.form.get("symbol"))

        if not results:
            # Return no results for that symbol
            return apology("no results for the symbol", 400)
        else:
            # Return results in quoted.html page
            return render_template("/quoted.html", results=results)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Require a user input a username
        if not request.form.get("username"):
            # Render an apology if username input is blank
            return apology("must provide username", 400)
        # Check if the user already exists
        elif len(db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))) > 0:
            # Render an apology if username input already exists
            return apology("username already exists", 400)
        # Require a user input a password
        elif not request.form.get("password"):
            # Render an apology if password is blank
            return apology("must provide password", 400)
        # Require a user input a password confirmation
        elif not request.form.get("confirmation"):
            # Render an apology if password confirmation is blank
            return apology("must provide password confirmation", 400)
        # Require the password and password confirmation to match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password and password confirmation do not match", 400)

        # Insert new user into users table, hashing the password
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)",
                   request.form.get("username"), generate_password_hash(request.form.get("password")))

        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Require that a user input a stock’s symbol, implemented as a select menu whose name is symbol.
        if not request.form.get("symbol"):
            # Render an apology if the user fails to select a stock
            return apology("Failed to select stock", 400)
        # Query to see if the user owns any shares of that stock
        elif len(db.execute("SELECT SUM(shares) FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol HAVING SUM(shares) > 0",
                            session["user_id"], request.form.get("symbol"))) == 0:
            # Render an apology if (somehow, once submitted) the user does not own any shares of that stock.
            return apology("No shares owned in selected stock", 400)

        # Require that a user input a number of shares, implemented as a text field whose name is shares.
        if int(request.form.get("shares")) < 0:
            # Render an apology if the input is not a positive integer
            return apology("Shares input must be a positive integer", 400)
        elif db.execute("SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol",
                        session["user_id"], request.form.get("symbol"))[0]["shares"] < int(request.form.get("shares")):
            # Render an apology if the user does not own that many shares of the stock.
            return apology("Insufficient amount of shares owned", 400)

        # Call lookup to look up a stock's current price
        sell_price = lookup(request.form.get("symbol"))["price"] * int(request.form.get("shares"))

        # Add sale to table
        db.execute("INSERT INTO transactions (user_id, symbol, price, shares, type) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], request.form.get("symbol"), sell_price, int(request.form.get("shares")) * -1, "SELL")

        # Add sell to user's cash
        db.execute("UPDATE users SET cash =  cash + ? WHERE id = ?", sell_price, session["user_id"])

        # When a sale is complete, redirect the user back to the index page
        return redirect("/")

    else:
        user_stocks = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])
        return render_template("sell.html", user_stocks=user_stocks)


# Personal Touch
@app.route("/change_password", methods=["GET", "POST"])
def change_password():
    if request.method == "POST":
        # Require a user input a username
        if not request.form.get("username"):
            # Render an apology if username input is blank
            return apology("must provide username", 400)
        # Check if the username exists
        elif len(db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))) == 0:
            # Render an apology if username input already exists
            return apology("username does not exists", 400)
        # Require a user input a password
        elif not request.form.get("current password"):
            # Render an apology if password is blank
            return apology("must provide password", 400)

        # Require them to input their old password
        current_password = db.execute("SELECT hash from users WHERE username = ?", request.form.get("username"))
        if check_password_hash(current_password[0]["hash"], request.form.get("current password")) is False:
            return apology("incorrect password", 400)

        # Require a user input a new password
        if not request.form.get("new password"):
            # Render an apology if password is blank
            return apology("must provide new password", 400)
        # Require a user input a new password confirmation
        elif not request.form.get("new password confirmation"):
            # Render an apology if password confirmation is blank
            return apology("must provide new password confirmation", 400)
        # Require that the new password doesn't match the current password
        elif check_password_hash(current_password[0]["hash"], request.form.get("new password")):
            return apology("new password must be different from current password", 400)
        # Require the password and password confirmation to match
        elif request.form.get("new password") != request.form.get("new password confirmation"):
            return apology("new password and new password confirmation do not match", 400)

        # Insert new user into users table, hashing the password
        db.execute("UPDATE users SET hash = ? WHERE username = ?",
                   generate_password_hash(request.form.get("new password")), request.form.get("username"))

        # Redirect them to the login page
        return redirect("/login")

    else:
        # Render a change password form
        return render_template("change_password.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
