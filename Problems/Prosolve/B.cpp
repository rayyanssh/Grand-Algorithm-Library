#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s;
    string t;

    cin >> s;
    cin >> t;
    s += s;
    auto index = s.find(t);
    if (index != string::npos && index < t.size())
    {
        cout << index;
    }
    else
    {
        cout << -1;
    }
    return 0;
}