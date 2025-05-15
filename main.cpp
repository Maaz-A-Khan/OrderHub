#include <iostream>
#include <vector>
using namespace std;

class Product{
	int id;
	string name;
	float price;
public:
	Product(int i=0, string n="", float p=0){
		id=i;
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
	string getPaymentType()const{return paymentType;};
	float getAmountPaid()const{return amountPaid;};
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
			products={};
			orders={};
		//will load products and orders from database
	}

	public:
		void addProduct(const Product& p){products.push_back(p);}
		void addOrder(Order o){orders.push_back(o);}

		void editProduct(int id, const string& newName, float newPrice) {
			for (auto& p : products) {
				if (p.getId() == id) {
					p = Product(id, newName, newPrice);
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
			int id;
			string name;
			float price;
			cout<<"Enter Product ID: ";
			cin>>id;
			cout<<"Enter Product Name: ";
			cin.ignore();
			getline(cin,name);
			cout<<"Enter Price: ";
			cin>>price;
			store.addProduct(Product(id,name,price));
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




