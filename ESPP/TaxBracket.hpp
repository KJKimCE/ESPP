//
//  TaxBracket.hpp
//  ESPP
//
//  Created by Kyu Jin Kim on 10/7/20.
//

#ifndef TaxBracket_hpp
#define TaxBracket_hpp

#include <stdio.h>
#include <iostream>

using namespace std;

class TaxBracket {
public:
    double income;
    double shortTerm;
    double longTerm;
    
    TaxBracket() {}
    
    TaxBracket(double _income, double _shortTerm, double _longTerm) {
        income = _income;
        shortTerm = _shortTerm;
        longTerm = _longTerm;
    }
    
    void print() {
        cout << "Tax Brackets" << endl << "Income: " << income * 100 << "%" << endl << "Short Term: " << shortTerm * 100 << "%" << endl << "Long Term: " << longTerm * 100 << "%" << endl << endl;
    }
};

#endif /* TaxBracket_hpp */
