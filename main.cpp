#include<iostream>
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
		string getName(){return name;	}
		float getPrice(){return price;	}
		virtual void display(){
			cout<<"Name: "<<name<<endl<<"Price per unit: "<<price<<endl;
		}
};
class OrderItem: public Product{
	int quantity;
	public:
		OrderItem(int i=0, string n="",float p=0, int q=0): Product(i,n,p), quantity(q){
		}
		int getQuantity(){return quantity;}
		float totalPrice(){return getPrice()*quantity;}
		void display(){
			Product::display();
			cout<<"Subtotal: "<<totalPrice()<<endl;
		}
};