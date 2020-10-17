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
#include "Regime.hpp"

class Future {
private:
    class Benefit {
    public:
        string regimeType;
        double diversificationBenefit;
        double pastProceeds;
        double finalProceeds;
        double difference;
        
        Benefit(Model& model, TaxBracket& taxBracket, Regime& regime, Future* future) {
            regimeType = regime.regimeType;
            diversificationBenefit = regime.finalProceeds * model.diversificationBenefit * ((future->date - max(regime.dateFrom, model.today)).count() / 365.0);
            pastProceeds = regime.finalProceeds + diversificationBenefit;
            finalProceeds = pastProceeds * (1 + future->expectedReturn * (1 - taxBracket.longTerm));
            difference = future->regime.finalProceeds - finalProceeds;
        }
        
        void print() {
            cout << "Diversification Benefit - " << regimeType << endl;
            cout << "Diversification Benefit: " << diversificationBenefit << endl;
            cout << "Past Proceeds: " << pastProceeds << endl;
            cout << "Final Proceeds: " << finalProceeds << endl;
            cout << "Difference: " << difference << endl;
            cout << endl;
        }
    };
public:
    Regime regime;
    
    double expectedReturn;
    double predictedPrice;
    sys_days date;
    vector<Benefit> benefits;
    
    Future() {}
    
    Future(Model& model, TaxBracket& taxBracket, CurrentSales& cs, sys_days futureDate) {
        date = futureDate;
        expectedReturn = (date - model.today).count()/365.0 * .08;
        predictedPrice = model.currentPrice * (1 + expectedReturn);
        
        // Depending on the date, create a Regime class
        int i = 0;
        while (i < cs.regime_dates.size()) {
            if (date >= cs.regime_dates[i].second.first && date < cs.regime_dates[i].second.second) {
                if (cs.regime_dates[i].first == "Qualifying") regime = Qualifying(model, taxBracket, predictedPrice);
                else if (cs.regime_dates[i].first == "Disqualifying Long") regime = DisqualifyingLong(model, taxBracket, predictedPrice);
                else if (cs.regime_dates[i].first == "Disqualifying Short") regime = DisqualifyingShort(model, taxBracket, predictedPrice);
                break;
            }
            i++;
        }
        
        // Depending on the date, push back multiple benefits
        while (--i >= 0) {
            Regime reg = choose(cs, cs.regime_dates[i].first);
            benefits.push_back(Benefit(model, taxBracket, reg, this));
        }
        
    }
    
    Regime choose(CurrentSales& cs, string regimeType) {
        if (regimeType == "Qualifying") return cs.qualifying;
        else if (regimeType == "Disqualifying Long") return cs.disqualifyingLong;
        
        return cs.disqualifyingShort;
    }
    
    void print() {
        cout << "Future - " << regime.regimeType << endl;
        cout << "Date: " << date << endl;
        cout << "Expected Return: " << expectedReturn << endl;
        cout << "Predicted Price: " << predictedPrice << endl;
        cout << "Proceeds: " << regime.saleProceeds << endl;
        cout << "Profit: " << regime.saleProfit << endl;
        cout << "Income: " << regime.income << endl;
        cout << "Short Term: " << regime.shortTerm << endl;
        cout << "Long Term: " << regime.longTerm << endl;
        cout << "Tax: " << regime.tax << endl;
        cout << "Final Proceeds: " << regime.finalProceeds << endl;
        cout << endl;
    }
};

#endif /* FutureSale_hpp */
