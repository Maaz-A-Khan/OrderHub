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
        cout << "Cash on delivery of Rs." << amount << " selected" << endl;

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
	vector<Product> products;
	vector<Order> orders;
	Store(){
		products={};
		orders={};
		//will load products and orders from database
	}
	public:
		void addOrder(Order o){orders.push_back(o);}
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






