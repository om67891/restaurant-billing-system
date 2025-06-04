from flask import Flask, request, send_from_directory, abort, Response
import subprocess
import os

app = Flask(__name__, static_folder='.', static_url_path='')

BILLING_EXEC = os.path.join('..', 'backend', 'billing')  # adjust if needed

@app.route('/')
def index():
    return send_from_directory('.', 'index.html')

@app.route('/style.css')
def style():
    return send_from_directory('.', 'style.css')

@app.route('/script.js')
def script():
    return send_from_directory('.', 'script.js')

@app.route('/generate-bill', methods=['POST'])
def generate_bill():
    input_data = request.get_data(as_text=True)

    try:
        # Run billing --web mode, pass input_data on stdin
        proc = subprocess.run([BILLING_EXEC, '--web'],
                              input=input_data,
                              text=True,
                              capture_output=True,
                              timeout=10)
    except Exception as e:
        return f"Error running billing program: {e}", 500

    if proc.returncode != 0:
        return f"Billing program error:\n{proc.stderr}", 500

    return Response(proc.stdout, mimetype='text/plain')

if __name__ == '__main__':
    # Run on http://localhost:5000
    app.run(debug=True)
