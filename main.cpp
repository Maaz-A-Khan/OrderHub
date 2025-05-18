#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Product{
	static int nextId;
	int id;
	string name;
	float price;
public:
	Product(string n="", float p=0){
		id=nextId++;
		name=n;
		price=p;
	}
	void setName(string n){name=n;}
	void setPrice(float p){price=p;}
	int getId()const{return id;	}
	string getName()const{return name;	}
	float getPrice()const{return price;	}
	virtual void display()const{
		cout<<"Name: "<<name<<endl<<"Price per unit: "<<price<<endl;
	}
};

int Product::nextId = 1;

class OrderItem: public Product{
	int quantity;
public:
	OrderItem(int i=0, string n="",float p=0, int q=0): Product(i,n,p), quantity(q){
	}
	int getQuantity()const{return quantity;}
	float totalPrice()const{return getPrice()*quantity;}
	void display() const{
		Product::display();
		cout<<"Subtotal: "<<totalPrice()<<endl;
	}
};

class PaymentMethod{
protected:
	string paymentType;
	float amountPaid;

public:
	string getPaymentType()const{return paymentType;}
	float getAmountPaid()const{return amountPaid;}
	virtual void pay(float amount) = 0;
};

class CardPayment : public PaymentMethod {
    string cardNumber;
    string expiry;
    int cvv;

public:
    CardPayment() {
        paymentType = "Card";
		amountPaid = 0;
        }

    void pay(float amount){
        cout << "Enter Card Number: ";
        cin >> cardNumber;
        cout << "Enter Expiry Date (MM/YY): ";
        cin >> expiry;
        cout << "Enter CVV: ";
        cin >> cvv;
        amountPaid = amount;
        cout << "Card payment of Rs. " << amount << " processed successfully.\n";
    }

};

class CashPayment : public PaymentMethod {
public:
    CashPayment() {
        paymentType = "Cash";
        amountPaid = 0;
    }

    void pay(float amount){
		amountPaid = amount;
        cout << "Cash on delivery of Rs." << amount <<"selected" << endl;

    }
};

class EasyPaisaPayment : public PaymentMethod {
	int accountNumber;
public:
	EasyPaisaPayment() {
		paymentType = "EasyPaisa";
		amountPaid = 0;
	}

	void pay(float amount){
		amountPaid = amount;
		cout << "Enter EasyPaisa Account Number: ";
		cin >> accountNumber;
		cout << "EasyPaisa payment of Rs." << amount << " processed successfully.\n";
	}
};

class JazzCashPayment : public PaymentMethod {
	int accountNumber;
public:
	JazzCashPayment() {
		paymentType = "JazzCash";
		amountPaid = 0;
	}

	void pay(float amount){
		amountPaid = amount;
		cout << "Enter JazzCash Account Number: ";
		cin >> accountNumber;
		cout << "JazzCash payment of Rs." << amount << " processed successfully.\n";
	}
};

class Order{
	static int nextId;
	int orderId, userId;
	string status;
	vector<OrderItem> cart;
	PaymentMethod* paymentMethod;

public:
	Order(int uid){
		orderId=nextId++;
		userId=uid;
		status="Pending";
		paymentMethod = nullptr;
	}

	~Order(){
		delete paymentMethod;
	}
	const vector<OrderItem>& getCart() const { return cart; }
	int getId() const{return orderId;}
	int getUserId() const{return userId;}
	string getStatus() const {return status;}
	string getPaymentMethod() const{return paymentMethod->getPaymentType();}

	void setStatus(string stat){status=stat;}
	void setPaymentMethod(PaymentMethod* pm){
		paymentMethod = pm;
        paymentMethod->pay(calculateTotalPrice());
	}

	void addItem(OrderItem item){
		cart.push_back(item);
	}
	float calculateTotalPrice() const{
		float total=0;
		for(const auto& item : cart){
			total+=item.totalPrice();
		}
		return total;
	}
	void display() const {
		cout<<"Order ID: "<<orderId<<endl;
		cout<<"User ID: "<<userId<<endl;
		cout<<"Status: "<<status<<endl;
		cout<<"Payment Method: "<<paymentMethod->getPaymentType()<<endl;
		cout<<"Items in Cart:" <<endl;

		if (cart.empty()) {
			cout << "  No items in the cart." << endl;
		} else {
			for (const auto& item : cart) {
				item.display();
				cout << "-----------------" << endl;
			}
		}
		cout<<"Total Price: "<<calculateTotalPrice()<<endl;
	}
};

int Order::nextId = 1;

class Store{
	protected:
		vector<Product> products;
		vector<Order> orders;
	private:
		Store(){
        loadProducts();
        loadOrders();
	    }
	    void loadProducts(){
	        ifstream fin("products.bin", ios::binary);
	        if (!fin) {
	            ofstream fout("products.bin", ios::binary); // create file
	            return;
	        }
	        int id;
	        float price;
	        size_t len;
	        string name;
	        while (fin.read(reinterpret_cast<char*>(&id), sizeof(id))) {
	            fin.read(reinterpret_cast<char*>(&len), sizeof(len));
	            name.resize(len);
	            fin.read(&name[0], len);
	            fin.read(reinterpret_cast<char*>(&price), sizeof(price));
	            products.emplace_back(id, name, price);
	        }
	        fin.close();
	    }
	    void loadOrders(){
	        ifstream fin("orders.bin", ios::binary);
	        if (!fin) {
	            ofstream fout("orders.bin", ios::binary); // create file
	            return;
	        }
	        int oid, uid, cartSize, pid, qty;
	        float price;
	        size_t len;
	        string status, name;
	        while (fin.read(reinterpret_cast<char*>(&oid), sizeof(oid))) {
	            fin.read(reinterpret_cast<char*>(&uid), sizeof(uid));
	            fin.read(reinterpret_cast<char*>(&len), sizeof(len));
	            status.resize(len);
	            fin.read(&status[0], len);
	            Order order(uid);
	            order.setStatus(status);

	            fin.read(reinterpret_cast<char*>(&cartSize), sizeof(cartSize));
	            for (int i = 0; i < cartSize; ++i) {
	                fin.read(reinterpret_cast<char*>(&pid), sizeof(pid));
	                fin.read(reinterpret_cast<char*>(&len), sizeof(len));
	                name.resize(len);
	                fin.read(&name[0], len);
	                fin.read(reinterpret_cast<char*>(&price), sizeof(price));
	                fin.read(reinterpret_cast<char*>(&qty), sizeof(qty));
	                order.addItem(OrderItem(pid, name, price, qty));
	            }
	            orders.push_back(order);
	        }
	        fin.close();
	    }

	public:
	    static Store& getInstance() {
	        static Store instance;
	        return instance;
	    }

	    Store(Store const&) = delete;
	    Store(Store&&) = delete;
	    Store& operator=(Store const&) = delete;
	    Store& operator=(Store&&) = delete;

		~Store(){
        saveProducts();
        saveOrders();
    	}

	    void saveProducts(){

	        ofstream fout("products.bin", ios::binary | ios::trunc);
	        for (const auto& p : products){
	            int id = p.getId();
	            string name = p.getName();
	            float price = p.getPrice();
	            size_t len = name.size();
	            fout.write(reinterpret_cast<char*>(&id), sizeof(id));
	            fout.write(reinterpret_cast<char*>(&len), sizeof(len));
	            fout.write(name.c_str(), len);
	            fout.write(reinterpret_cast<char*>(&price), sizeof(price));
	        }
	        fout.close();
	    }

	    void saveOrders(){
	        ofstream fout("orders.bin", ios::binary | ios::trunc);
	        for (const auto& o : orders){
	            int oid = o.getId();
	            int uid = o.getUserId();
	            string status = o.getStatus();
	            size_t len = status.size();
	            fout.write(reinterpret_cast<char*>(&oid), sizeof(oid));
	            fout.write(reinterpret_cast<char*>(&uid), sizeof(uid));
	            fout.write(reinterpret_cast<char*>(&len), sizeof(len));
	            fout.write(status.c_str(), len);

	            const vector<OrderItem>& cart = o.getCart();
	            int cartSize = cart.size();
	            fout.write(reinterpret_cast<char*>(&cartSize), sizeof(cartSize));

	            for (const auto& item : cart){
	                int pid = item.getId();
	                string name = item.getName();
	                float price = item.getPrice();
	                int qty = item.getQuantity();
	                len = name.size();
	                fout.write(reinterpret_cast<char*>(&pid), sizeof(pid));
	                fout.write(reinterpret_cast<char*>(&len), sizeof(len));
	                fout.write(name.c_str(), len);
	                fout.write(reinterpret_cast<char*>(&price), sizeof(price));
	                fout.write(reinterpret_cast<char*>(&qty), sizeof(qty));
	            }
	        }
	        fout.close();
	    }
		void addProduct(const Product& p){products.push_back(p);}
		void addOrder(Order o){orders.push_back(o);}

		void editProduct(int id, const string& newName, float newPrice) {
			for (auto& p : products) {
				if (p.getId() == id) {
					p = Product(newName, newPrice);
					break;
				}
			}
   		}

		Product* getProductById(int id) {
		    if (products.empty()) {
		        cout << "No products available." << endl;
		        return nullptr;
		    }
		    for (auto& prod : products) {
		        if (prod.getId()==id) {
		            return &prod;
		        }
		    }
		    cout << "Product not found." << endl;
		    return nullptr;
		}

		void displayProducts(){
			if(products.empty())
				cout<<" No Products "<< endl;
			else{
				for (const auto& prod : products){
					prod.display();
					cout << "-----------------" << endl;
				}
			}
		}
		void displayOrders(){
			if(orders.empty())
				cout<<" No Orders"<<endl;
			else{
				for (const auto& ord : orders){
					ord.display();
					cout << "-----------------" << endl;
				}
			}

		}
		void displayUserOrders(int userId) const {
			if (orders.empty()) {
			cout << "No orders found." << endl;
			return;
		}
        bool found = false;
        for (const auto& o : orders) {
            if (o.getUserId() == userId) {
                o.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No orders found for User ID " << userId << ".\n";
        }
    }

		void updateOrderStatus(int orderId, const string& status) {
        for (auto& o : orders) {
            if (o.getId() == orderId) {
                o.setStatus(status);
                cout << "Order status updated.\n";
                return;
            }
        }
        cout << "Order not found.\n";
    	}

		void trackOrder(int orderId){
			if(orders.empty())
				cout<<" No Orders"<<endl;
			else{
				bool found = false;
				for (const auto& ord : orders){
				    if(ord.getId()==orderId){
				        cout<<" Order ID= "<<ord.getId()<<endl<<" Status= "<<ord.getStatus() << endl;
				        cout << "-----------------" << endl;
				        found = true;
				        break;
				    }
				}
				if (!found) cout<<" Order not Found"<<endl;
			}
		}


};
class Admin{
	public:
		void viewProducts(Store& store){
			cout<<"All Products:\n";
			store.displayProducts();
		}

		void addProduct(Store& store){
			string name;
			float price;
			cout<<"Enter Product Name: ";
			cin>>name;
			cout<<"Enter Price: ";
			cin>>price;
			store.addProduct(Product(name,price));
			cout<<"Product added!\n";
		}
		void editProduct(Store& store){
			int id;
			string name;
			float price;
			cout<<"Enter Product ID to edit: ";
			cin>>id;
			cout<<"New Name: ";
			cin.ignore();
			getline(cin,name);
			cout<<"New Price: ";
			cin>>price;
			store.editProduct(id,name,price);
			cout<<"Product updated!\n";
		}
		void updateOrderStatus(Store& store){
			int orderId;
			string status;
			cout << "Enter Order ID to update: ";
			cin >> orderId;
			cout << "New Status: ";
			cin>>status;
			store.updateOrderStatus(orderId, status);
		}
};

class User{
	int id;
	string name;
public:
	User(int i=0, string n=""){
		id=i;
		name=n;
	}
	int getId() const{return id;}
	string getName() const{return name;}
	void viewProducts(Store& store){
		cout<<"All Products:\n";
		store.displayProducts();
	}
	void placeOrder(Store& store) {
        Order order(id);
        int pid, qty;
        char choice;

        do {
            cout << "Enter Product ID to add: ";
            cin >> pid;
            cout << "Quantity: ";
            cin >> qty;
            Product* product = store.getProductById(pid);
            if (product) {
                order.addItem(OrderItem(pid, product->getName(), product->getPrice(), qty));
            } else {
                cout << "Product not found.\n";
            }
            cout << "Add more items? (y/n): ";
            cin >> choice;
        } while (choice == 'y');

        order.display();
        cout << "Proceed with payment? (y/n): ";
        cin >> choice;

        if (choice == 'y') {
            int paymentOption;
            PaymentMethod* payment = nullptr;
            cout << "Choose payment method (1. Card, 2. Cash, 3. EasyPaisa, 4. JazzCash): ";
            cin >> paymentOption;

            switch (paymentOption) {
                case 1: 
					payment = new CardPayment(); 
					break;
                case 2: 
					payment = new CashPayment(); 
					break;
				case 3:
					payment = new EasyPaisaPayment();
					break;
				case 4:
					payment = new JazzCashPayment();
					break;
                default: 
					cout << "Invalid option. Defaulting to Cash.\n"; 
					payment = new CashPayment();
            }

            order.setPaymentMethod(payment);
            store.addOrder(order);
            cout << "Order placed successfully!\n";
        } else {
            cout << "Order cancelled.\n";
        }
	}
	
	void trackOrder(Store& store) {
		int orderId;
		cout << "Enter Order ID to track: ";
		cin >> orderId;
		store.trackOrder(orderId);
	}

	void viewMyOrders(Store& store) {
		store.displayUserOrders(id);
	}

};

int authenticator(string username,string password){
	if (username=="user123" && password=="user123"){
		return 1;
	}else 	if (username=="admin123" && password=="admin123"){
		return 2;
	}else{
		return 3;
	}
}