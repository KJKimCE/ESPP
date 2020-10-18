//
//  Model.hpp
//  ESPP
//
//  Created by Kyu Jin Kim on 10/7/20.
//

#ifndef Model_hpp
#define Model_hpp

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include "date/date.h"

using namespace std;
using namespace date;

class Model {
public:
    double grantPrice;
    double exercisePrice;
    double purchasePrice;
    double discountedPrice;
    double quantity;
    double currentPrice;
    
    sys_days grantDate;
    sys_days exerciseDate;
    sys_days purchaseDate;
    sys_days today;
    
    double transactionCost;
    double diversificationBenefit;
    
    Model() {}
    
    Model(double _grantPrice, double _exercisePrice, double _currentPrice, double _quantity, double _transactionCost, double _db, year_month_day _purchaseDate) {
        grantPrice = _grantPrice;
        exercisePrice = _exercisePrice;
        purchasePrice = min(grantPrice, exercisePrice);
        currentPrice = _currentPrice;
        discountedPrice = purchasePrice * (1 - 0.15);
        quantity = _quantity;
        
        purchaseDate = _purchaseDate;
        exerciseDate = _purchaseDate;
        grantDate = _purchaseDate.year()/(_purchaseDate.month() - months(2))/1;
        today = floor<days>(chrono::system_clock::now());
        
        transactionCost = _transactionCost;
        diversificationBenefit = _db;
    }
    
    void print() {
        cout << "Model:" << endl;
        cout << "Today: " << today << endl;
        cout << "Grant Date: " << grantDate << endl;
        cout << "Grant Price: " << grantPrice << endl;
        cout << "Exercise Date: " << exerciseDate << endl;
        cout << "Exercise Price: " << exercisePrice << endl;
        cout << "Purchase Date: " << purchaseDate << endl;
        cout << "Purchase Price: " << purchasePrice << endl;
        cout << "Discounted Price: " << discountedPrice << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Current Price: " << currentPrice << endl;
        cout << "Transaction Cost: " << transactionCost << endl;
        cout << "Diversification Benefit: " << diversificationBenefit * 100 << "%" << endl;
        cout << endl;
    }
};

#endif /* Model_hpp */
