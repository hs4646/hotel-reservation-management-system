#pragma once
#ifndef GUEST_H
#define GUEST_H

using namespace std;

class guest
{
public:
    guest() {};
    ~guest() {};
    void ViewRoom();
    void ReserveRoom();
    void MakePayment();
    void Profile();
    void GenerateReceipt(const string& reservationID, double amountPaid);

};
#endif