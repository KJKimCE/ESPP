//
//  CurrentSale.hpp
//  ESPP
//
//  Created by Kyu Jin Kim on 10/7/20.
//

#ifndef Regime_hpp
#define Regime_hpp

#include "Model.hpp"
#include "TaxBracket.hpp"

class Regime {
public:
    string regimeType;
    double saleProceeds;
    double saleProfit;
    double income;
    double shortTerm;
    double longTerm;
    double tax;
    double finalProceeds;
    
    sys_days dateFrom, dateTo;
    
    Regime() {}//cout << "base constructor called" << endl;}
    
    void initialize(Model& model, double price) {
        saleProceeds = model.quantity * price - model.transactionCost;
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

class DisqualifyingShort : public Regime {
private:
    void setInfo(Model& model) {
        regimeType = "Disqualifying Short";
        dateFrom = model.exerciseDate + days{45};
        dateTo = model.purchaseDate + days{365} + days{45};
        
        income = model.quantity * (model.exercisePrice - model.discountedPrice);
        shortTerm = saleProfit - income;
        longTerm = 0;
    }
public:
    DisqualifyingShort() {}
    
    DisqualifyingShort(Model& model, TaxBracket& taxBracket, double price) {
        //cout << "Disqualifying Short Constructor Called" << endl;
        initialize(model, price);
        this->setInfo(model);
        calculateTaxes(taxBracket);
    }
    
    void print() {
        cout << "Disqualifying Short:" << endl;
        Regime::print();
    }
};

class DisqualifyingLong : public Regime {
private:
    void setInfo(Model& model) {
        regimeType = "Disqualifying Long";
        dateFrom = model.purchaseDate + days{365} + days{45};
        dateTo = max(model.grantDate + days{730}, model.exerciseDate + days{365}) + days{15};
        
        income = model.quantity * (model.exercisePrice - model.discountedPrice);
        shortTerm = 0;
        longTerm = saleProfit - income;
    }
public:
    DisqualifyingLong() {}
    
    DisqualifyingLong(Model& model, TaxBracket& taxBracket, double price) {
//        cout << "Disqualifying Long Constructor Called" << endl;
        initialize(model, price);
        this->setInfo(model);
        calculateTaxes(taxBracket);
    }
    
    void print() {
        cout << "Disqualifying Long:" << endl;
        Regime::print();
    }
};

class Qualifying : public Regime {
private:
    void setInfo(Model& model) {
        regimeType = "Qualifying";
        dateFrom = max(model.grantDate + days{730}, model.exerciseDate + days{365}) + days{15};
        dateTo = year(9999)/12/31;
        
        income = model.quantity * max(min(model.currentPrice - model.discountedPrice - model.transactionCost, .15 * model.grantPrice), 0.0);
        shortTerm = 0;
        longTerm = saleProfit - income;
    }
public:
    Qualifying() {}
    
    Qualifying(Model& model, TaxBracket& taxBracket, double price) {
//        cout << "Qualifying Constructor Called" << endl;
        initialize(model, price);
        this->setInfo(model);
        calculateTaxes(taxBracket);
    }
    
    void print() {
        cout << "Qualifying:" << endl;
        Regime::print();
    }
};

class CurrentSales {
public:
    DisqualifyingShort disqualifyingShort;
    DisqualifyingLong disqualifyingLong;
    Qualifying qualifying;
    vector<pair<string, pair<sys_days, sys_days>>> regime_dates;
    
    CurrentSales() {}
    
    CurrentSales(Model& model, TaxBracket& taxBracket) {
        disqualifyingShort = DisqualifyingShort(model, taxBracket, model.currentPrice);
        disqualifyingLong = DisqualifyingLong(model, taxBracket, model.currentPrice);
        qualifying = Qualifying(model, taxBracket, model.currentPrice);
        
        regime_dates.push_back(make_pair("Disqualifying Short", make_pair(disqualifyingShort.dateFrom, disqualifyingShort.dateTo)));
        regime_dates.push_back(make_pair("Disqualifying Long", make_pair(disqualifyingLong.dateFrom, disqualifyingLong.dateTo)));
        regime_dates.push_back(make_pair("Qualifying", make_pair(qualifying.dateFrom, qualifying.dateTo)));
    }
    
    void print() {
        disqualifyingShort.print();
        disqualifyingLong.print();
        qualifying.print();
    }
};

#endif /* CurrentSale_hpp */
