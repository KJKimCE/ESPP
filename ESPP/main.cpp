//
//  main.cpp
//  ESPP
//
//  Created by Kyu Jin Kim on 10/7/20.
//

#include <iostream>
#include <vector>
#include "Model.hpp"
#include "Regime.hpp"
#include "FutureSale.hpp"
#include "TaxBracket.hpp"

using namespace date;

Model buildModel() {
    double grantPrice, exercisePrice, currentPrice, quantity, transactionCost, db;
    int y, m, d;
    
    cout << "Enter grant price: ";
    cin >> grantPrice;
    cout << "Enter Exercise Price: ";
    cin >> exercisePrice;
    cout << "Enter Current Price: ";
    cin >> currentPrice;
    cout << "Enter Quantity: ";
    cin >> quantity;
    cout << "Enter Purchase Date: Year, Month, Date" << endl;
    cin >> y >> m >> d;
    cout << "Enter Transaction Cost: ";
    cin >> transactionCost;
    cout << "Enter Diversification Benefit: ";
    cin >> db;
    
    year_month_day purchaseDate = year(y)/m/d;
    
    return Model(grantPrice, exercisePrice, currentPrice, quantity, transactionCost, db, purchaseDate);
}

TaxBracket buildTaxBracket() {
    double income, shortTerm, longTerm;
    
    cout << "Enter Tax Bracket % (xx.xx) - Income: ";
    cin >> income;
    cout << "Enter Tax Bracket % (xx.xx) - Short Term: ";
    cin >> shortTerm;
    cout << "Enter Tax Bracket % (xx.xx) - Long Term: ";
    cin >> longTerm;
    
    return TaxBracket(income / 100, shortTerm / 100, longTerm / 100);
}

int main(int argc, const char * argv[]) {
    Model model;
    TaxBracket taxBracket;
    int test = 0;
    cout << "Sample? ";
    cin >> test;
    
    if (!test) {
        // 47, 60.90, 70, 1, 10
        model = buildModel();
        // 31.82, 30, 21
        taxBracket = buildTaxBracket();
    } else {
//        year_month_day ymd = floor<days>(chrono::system_clock::now());
        year_month_day ymd = (year)2020/3/30;
        model = Model(47, 60.90, 70, 1, 0, .01, ymd);
        taxBracket = TaxBracket(.3182, .3, .21);
    }
    
    cout << endl;
    
    model.print();
    taxBracket.print();
    
    CurrentSales csm = CurrentSales(model, taxBracket);
    csm.print();
    
    int y, m, d;
    cout << "Enter Purchase Date: Year, Month, Date" << endl;
    cin >> y >> m >> d;
    
    Future future = Future(model, taxBracket, csm, (year)y/m/d);
    future.print();
    
    for (auto& b : future.benefits) b.print();
    
    return 0;
}
