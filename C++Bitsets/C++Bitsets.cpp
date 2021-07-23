#include <bitset>
#include <iostream>
#include <vector>
#include <random>
using namespace std;


int main() {

    unsigned char c = 202;
    cout << sizeof(unsigned char) << endl;
    cout << (int)c << endl;

    c = ~c;

    cout << (int)c << endl;

    string s1 = "1100";
    bitset<4> b1(s1);
    string s2 = "0110";
    bitset<4> b2(s2);

    cout << "b1   " << b1 << endl;
    cout << "b2   " << b2 << endl;

    b1 = b1 | b2;
    // b1 |= b2;

    cout << "OR|) " << b1 << endl;

    b1 = bitset<4>("1100");

    cout << "b1    " << b1 << endl;
    cout << "b2    " << b2 << endl;

    b1 = b1 ^ b2;
    // b1 ^= b2;

    cout << "XOR^) " << b1 << endl;

    uint8_t a = 4;
    b1 = a;
    cout << "uint8_t a " << b1 << endl;

    int p = 5;
    int q = 3;

    p = p ^ q;

    cout << "p = " << p << endl;
    std::vector<int> bits;
    std::random_device rnd;     // 非決定的な乱数生成器を生成
    std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    std::uniform_int_distribution<> rand100(0, 1);        // [0, 99] 範囲の一様乱数
    for (int i = 0; i < 16; i++)
        bits.emplace_back(rand100(mt));


    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
        {
            cout << bits[i * 4 + j];
        }
        cout << endl;
    }


    for (int i = 0; i < 16; i++)
    {
        if (bits[i] == 1)
            cout << i << endl;
    }
    int result = 0;
    while (result != 16)
    {
        result = 0;

        for (int i = 0; i < 16; i++)
            if (bits[i] == 1)
                result = result ^ i;
        cout << "result = " << result << endl;

        int temp;
        cin >> temp;
        bits[temp] = !bits[temp];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
            {
                cout << bits[i * 4 + j];
            }
            cout << endl;
        }
    }



    return 0;
}