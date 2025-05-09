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
	string paymentType;
	float amountPaid;
public:
	virtual string getPaymentType()const{return paymentType;};
	virtual float getAmountPaid()const{return amountPaid;};
	virtual void pay(float amount) = 0;
};

// Card,Cash,and EasyPaisa classes


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






