#  Restaurant Billing System (Terminal + Web UI)

This is a simple restaurant billing system built in C, with a modern web interface using HTML, CSS, and JavaScript.  
It lets you generate clean bills based on customer orders — either from the terminal or through a web browser!

---

##  Features

- 📋 Add multiple items with quantity and price
- 🧮 Calculates subtotal, discount, taxes (CGST & SGST), and grand total
- 🖥️ Works via terminal (stdin) or via a web interface
- 🌐 Web interface built with HTML + CSS + JS
- ⚙️ Backend in C, connected through a Python Flask server
- 💾 Clean code structure — easy to extend

---

##  Project Structure

restaurant-billing-system/
├── backend/
│ ├── billing.c # C billing logic
│ └── billing.exe # Executable (generated)
├── web/
│ ├── index.html # Web UI
│ ├── style.css # Styling
│ ├── script.js # JS logic
│ └── server.py # Python server (Flask)
├── invoices/ # (Optional) Future storage for bills
├── README.md # This file :)
└── .gitignore # Ignore compiled/output files


---

##  How to Run

###  Terminal Mode (for C developers)

cd backend
gcc billing.c -o billing
./billing

🌐 Web Mode (with Flask)
Make sure you have Python & Flask installed:
pip install flask

Then run the server:

cd web
python server.py

