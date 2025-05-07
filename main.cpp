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
	int getId(){return id;	}
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


class Order{
	static int nextId;
	int orderId, userID;
	string status;
	vector<OrderItem> cart;
	PaymentMethod* paymentMethod;
	
public:
	Order(int uid){
		orderId=nextId++;
		userId=uid;
		status="Pending";
	}
	
	int getId() const{return orderId;}
	int getUserId() const{return userId;}
	string getStatus() const {return status;}
	string getPaymentMethod() const{return paymentMethod->getPaymentType();}
	void setStatus(string stat){status=stat;}
	// setPayment method (to be after PaymentMethod class is implemented)

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
		cout<<"User ID: "<<userID<<endl;
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
