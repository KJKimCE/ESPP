//
//  FutureSale.hpp
//  ESPP
//
//  Created by Kyu Jin Kim on 10/15/20.
//

#ifndef FutureSale_hpp
#define FutureSale_hpp

#include "Model.hpp"
#include "TaxBracket.hpp"
#include "CurrentSale.hpp"

class FutureSale {
public:
    Model model;
    CurrentSale currentSale;
    TaxBracket taxBracket;
    
    double expectedReturn;
    double predictedPrice;
    sys_days date;
    
    double proceeds;
    double profit;
    double income;
    double shortTerm;
    double longTerm;
    double tax;
    double finalProceeds;
    
    FutureSale() {}
    
    void setObjects(Model& _model, CurrentSale current, TaxBracket tb) {
        model = _model;
        currentSale = current;
        taxBracket = tb;
    }
    
    void calcProfit() {
        expectedReturn = (currentSale.dateFrom - model.today).count()/365.0 * .08;
        predictedPrice = model.currentPrice * (1 + expectedReturn);
        proceeds = predictedPrice * model.quantity - model.transactionCost;
        profit = proceeds - model.discountedPrice * model.quantity;
    }
    
    void calcFinalProceeds() {
        shortTerm = 0;
        longTerm = profit - income - shortTerm;
        tax = income * taxBracket.income + shortTerm * taxBracket.shortTerm + longTerm * taxBracket.longTerm;
        finalProceeds = proceeds - tax;
    }
    
    void print() {
        cout << "Date: " << date << endl;
        cout << "Expected Return: " << expectedReturn << endl;
        cout << "Predicted Price: " << predictedPrice << endl;
        cout << "Proceeds: " << proceeds << endl;
        cout << "Profit: " << profit << endl;
        cout << "Income: " << income << endl;
        cout << "Short Term: " << shortTerm << endl;
        cout << "Long Term: " << longTerm << endl;
        cout << "Tax: " << tax << endl;
        cout << "Final Proceeds: " << finalProceeds << endl;
        cout << endl;
    }
};

class DisqualifyingLongFuture : public FutureSale {
public:
    DisqualifyingLongFuture() {}
    
    DisqualifyingLongFuture(Model& _model, DisqualifyingLong dl, TaxBracket tb) {
        setObjects(_model, dl, tb);
        
        date = max(model.today, currentSale.dateFrom);
        calcProfit();
        income = currentSale.income;
        calcFinalProceeds();
    }
    
    void print() {
        cout << "Disqualifying Long Future:" << endl;
        FutureSale::print();
    }
};

class QualifyingFuture : public FutureSale {
public:
    QualifyingFuture() {}
    
    QualifyingFuture(Model& _model, Qualifying qualifying, TaxBracket tb) {
        setObjects(_model, qualifying, tb);
        
        date = currentSale.dateFrom;
        calcProfit();
        income = min(model.quantity * (predictedPrice - model.discountedPrice) - model.transactionCost, model.quantity * (model.purchasePrice - model.discountedPrice));
        calcFinalProceeds();
    }
    
    void print() {
        cout << "Qualifying Future:" << endl;
        FutureSale::print();
    }
};

class Benefit {
public:
    double diversificationBenefit;
    double pastProceeds;
    double finalProceeds;
};

#endif /* FutureSale_hpp */
