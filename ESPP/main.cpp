//
//  main.cpp
//  ESPP
//
//  Created by Kyu Jin Kim on 10/7/20.
//

#include <iostream>
#include <vector>
#include "Model.hpp"
#include "CurrentSale.hpp"
#include "FutureSale.hpp"
#include "TaxBracket.hpp"

using namespace date;

Model buildModel() {
    double grantPrice, exercisePrice, currentPrice, quantity, transactionCost;
    int y, m, d;
    
    cout << "Enter grant price: ";
    cin >> grantPrice;
    cout << "Enter Exercise Price: ";
    cin >> exercisePrice;
    cout << "Enter Current Price: ";
    cin >> currentPrice;
    cout << "Enter Quantity: ";
    cin >> quantity;
    cout << "Enter Transaction Cost: ";
    cin >> transactionCost;
    cout << "Enter Purchase Date: Year, Month, Date" << endl;
    cin >> y >> m >> d;
    
    year_month_day purchaseDate = year(y)/m/d;
    
    return Model(grantPrice, exercisePrice, currentPrice, quantity, transactionCost, purchaseDate);
}

TaxBracket buildTaxBracket() {
    double income, shortTerm, longTerm;
    
    cout << "Enter Tax Bracket % (xx.xx) - Income: ";
    cin >> income;
    cout << "Enter Tax Bracket % (xx.xx) - Short Term: ";
    cin >> shortTerm;
    cout << "Enter Tax Bracket % (xx.xx) - Long Term: ";
    cin >> longTerm;
    
    return TaxBracket(income, shortTerm, longTerm);
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
        model = Model(47, 60.90, 70, 1, 0, ymd);
        taxBracket = TaxBracket(31.82, 30, 21);
    }
    
    cout << endl;
    
    model.print();
    taxBracket.print();
    
    CurrentSaleModel csm = CurrentSaleModel(model, taxBracket);
    csm.print();
    
    DisqualifyingLongFuture dlf = DisqualifyingLongFuture(model, csm.disqualifyingLong, taxBracket);
    dlf.print();
    
    QualifyingFuture qf = QualifyingFuture(model, csm.qualifying, taxBracket);
    qf.print();
    
    return 0;
}
