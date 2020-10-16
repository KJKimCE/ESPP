//
//  CurrentSale.hpp
//  ESPP
//
//  Created by Kyu Jin Kim on 10/7/20.
//

#ifndef CurrentSale_hpp
#define CurrentSale_hpp

#include "Model.hpp"
#include "TaxBracket.hpp"

class CurrentSale {
public:
    double saleProceeds;
    double saleProfit;
    double income;
    double shortTerm;
    double longTerm;
    double tax;
    double finalProceeds;
    
    sys_days dateFrom, dateTo;
    
    CurrentSale() {}//cout << "base constructor called" << endl;}
    
    void initialize(Model& model) {
        saleProceeds = model.quantity * model.currentPrice - model.transactionCost;
        saleProfit = saleProceeds - model.quantity * model.discountedPrice;
    }
    
    void calculateTaxes(TaxBracket& taxBracket) {
        tax = income * taxBracket.income + shortTerm * taxBracket.shortTerm + longTerm * taxBracket.longTerm;
        finalProceeds = saleProceeds - tax;
    }
    
    void print() {
        cout << "Date From: " << dateFrom << endl;
        cout << "Date To: " << dateTo << endl;
        cout << "Sale Proceeds: " << saleProceeds << endl;
        cout << "Sale Profit: " << saleProfit << endl;
        cout << "Income: " << income << endl;
        cout << "Short Term: " << shortTerm << endl;
        cout << "Long Term: " << longTerm << endl;
        cout << "Tax: " << tax << endl;
        cout << "Final Proceeds: " << finalProceeds << endl;
        cout << endl;
    }
};

class DisqualifyingShort : public CurrentSale {
public:
    DisqualifyingShort() {}
    
    DisqualifyingShort(Model& model, TaxBracket& taxBracket) {
        //cout << "Disqualifying Short Constructor Called" << endl;
        initialize(model);
        setInfo(model);
        calculateTaxes(taxBracket);
    }
    
    void setInfo(Model& model) {
        dateFrom = model.exerciseDate + days{45};
        dateTo = model.purchaseDate + days{365} + days{45};
        
        income = model.quantity * (model.exercisePrice - model.discountedPrice);
        shortTerm = saleProfit - income;
        longTerm = 0;
    }
    
    void print() {
        cout << "Disqualifying Short:" << endl;
        CurrentSale::print();
    }
};

class DisqualifyingLong : public CurrentSale {
public:
    DisqualifyingLong() {}
    
    DisqualifyingLong(Model& model, TaxBracket& taxBracket) {
//        cout << "Disqualifying Long Constructor Called" << endl;
        initialize(model);
        setInfo(model);
        calculateTaxes(taxBracket);
    }

    void setInfo(Model& model) {
        dateFrom = model.purchaseDate + days{365} + days{45};
        dateTo = max(model.grantDate + days{730}, model.exerciseDate + days{365}) + days{15};
        
        income = model.quantity * (model.exercisePrice - model.discountedPrice);
        shortTerm = 0;
        longTerm = saleProfit - income;
    }
    
    void print() {
        cout << "Disqualifying Long:" << endl;
        CurrentSale::print();
    }
};

class Qualifying : public CurrentSale {
public:
    Qualifying() {}
    
    Qualifying(Model& model, TaxBracket& taxBracket) {
//        cout << "Qualifying Constructor Called" << endl;
        initialize(model);
        setInfo(model);
        calculateTaxes(taxBracket);
    }

    void setInfo(Model& model) {
        dateFrom = max(model.grantDate + days{730}, model.exerciseDate + days{365}) + days{15};
        dateTo = year(9999)/12/31;
        
        income = model.quantity * max(min(model.currentPrice - model.discountedPrice - model.transactionCost, .15 * model.grantPrice), 0.0);
        shortTerm = 0;
        longTerm = saleProfit - income;
    }
    
    void print() {
        cout << "Qualifying:" << endl;
        CurrentSale::print();
    }
};

class CurrentSaleModel {
public:
    DisqualifyingShort disqualifyingShort;
    DisqualifyingLong disqualifyingLong;
    Qualifying qualifying;
    
    CurrentSaleModel() {}
    
    CurrentSaleModel(Model& model, TaxBracket& taxBracket) {
        disqualifyingShort = DisqualifyingShort(model, taxBracket);
        disqualifyingLong = DisqualifyingLong(model, taxBracket);
        qualifying = Qualifying(model, taxBracket);
    }
    
    void print() {
        disqualifyingShort.print();
        disqualifyingLong.print();
        qualifying.print();
    }
};

#endif /* CurrentSale_hpp */
