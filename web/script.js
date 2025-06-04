const form = document.getElementById('billForm');
const itemsContainer = document.getElementById('itemsContainer');
const addItemBtn = document.getElementById('addItemBtn');
const output = document.getElementById('output');
const numItemsInput = document.getElementById('numItems');

// Add another item input group
addItemBtn.addEventListener('click', () => {
  const itemDiv = document.createElement('div');
  itemDiv.className = 'item';
  itemDiv.innerHTML = `
    <label>Item Name:</label>
    <input type="text" class="itemName" required />
    <label>Quantity:</label>
    <input type="number" class="itemQty" min="1" value="1" required />
    <label>Unit Price:</label>
    <input type="number" class="itemPrice" min="0" step="0.01" value="0" required />
  `;
  itemsContainer.appendChild(itemDiv);
  updateNumItems();
});

function updateNumItems() {
  const count = itemsContainer.querySelectorAll('.item').length;
  numItemsInput.value = count;
}

// Keep numItems synced if user manually edits
numItemsInput.addEventListener('change', () => {
  let count = parseInt(numItemsInput.value);
  if (isNaN(count) || count < 1) {
    count = 1;
    numItemsInput.value = 1;
  }

  const currentCount = itemsContainer.querySelectorAll('.item').length;

  if (count > currentCount) {
    for (let i = currentCount; i < count; i++) {
      addItemBtn.click();
    }
  } else if (count < currentCount) {
    for (let i = currentCount; i > count; i--) {
      itemsContainer.removeChild(itemsContainer.lastElementChild);
    }
  }
});

form.addEventListener('submit', async (e) => {
  e.preventDefault();

  const customer = document.getElementById('customer').value.trim();
  const numItems = parseInt(numItemsInput.value);
  const items = itemsContainer.querySelectorAll('.item');

  let inputText = customer + '\n' + numItems + '\n';

  for (let i = 0; i < numItems; i++) {
    const name = items[i].querySelector('.itemName').value.trim();
    const qty = items[i].querySelector('.itemQty').value;
    const price = items[i].querySelector('.itemPrice').value;

    inputText += name + '\n' + qty + '\n' + price + '\n';
  }

  try {
    const response = await fetch('/generate-bill', {
      method: 'POST',
      headers: {
        'Content-Type': 'text/plain',
      },
      body: inputText,
    });

    if (!response.ok) {
      throw new Error(`Server error: ${response.statusText}`);
    }

    const billText = await response.text();
    output.textContent = billText;
  } catch (err) {
    output.textContent = 'Error: ' + err.message;
  }
});
