#include <iostream>
#include <cstring>
#include <stdexcept>

class String {
protected:
    char* data;
    size_t length;

public:
    String() : data(nullptr), length(0) {}

    String(const char* str) {
        if (str) {
            length = std::strlen(str);
            data = new char[length + 1];
            std::strcpy(data, str);
        }
        else {
            data = nullptr;
            length = 0;
        }
    }

    String(const String& other) : data(nullptr), length(other.length) {
        if (other.data) {
            data = new char[length + 1];
            std::strcpy(data, other.data);
        }
    }

    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            if (other.data) {
                data = new char[length + 1];
                std::strcpy(data, other.data);
            }
            else {
                data = nullptr;
            }
        }
        return *this;
    }

    size_t getLength() const {
        return length;
    }

    void clear() {
        delete[] data;
        data = nullptr;
        length = 0;
    }

    virtual ~String() {
        delete[] data;
    }

    String operator+(const String& other) const {
        size_t newLength = length + other.length;
        char* newData = new char[newLength + 1];
        std::strcpy(newData, data ? data : "");
        std::strcat(newData, other.data ? other.data : "");

        String result(newData);
        delete[] newData;
        return result;
    }

    String& operator+=(const String& other) {
        *this = *this + other;
        return *this;
    }

    bool operator==(const String& other) const {
        return std::strcmp(data ? data : "", other.data ? other.data : "") == 0;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }
};

class BitString : public String {
public:
    BitString() : String() {}

    BitString(const char* str) : String() {
        if (str && isValidBitString(str)) {
            length = std::strlen(str);
            data = new char[length + 1];
            std::strcpy(data, str);
        }
        else {
            clear();
        }
    }

    BitString(const BitString& other) : String(other) {}

    BitString& operator=(const BitString& other) {
        String::operator=(other);
        return *this;
    }
    ~BitString() override = default;

    void negate() {
        if (!data) return;

        for (size_t i = 0; i < length; ++i) {
            data[i] = (data[i] == '0') ? '1' : '0';
        }

        bool carry = true;
        for (size_t i = length; i-- > 0 && carry;) {
            if (data[i] == '1') {
                data[i] = '0';
            }
            else {
                data[i] = '1';
                carry = false;
            }
        }
    }

    BitString operator+(const BitString& other) const {

        BitString result((String::operator+(other)).data);
        if (!isValidBitString(result.data)) {
            result.clear();
        }
        return result;
    }

    BitString& operator+=(const BitString& other) {
        *this = *this + other;
        return *this;
    }


    bool operator==(const BitString& other) const {
        return String::operator==(other);
    }


    bool operator!=(const BitString& other) const {
        return String::operator!=(other);
    }

private:
    bool isValidBitString(const char* str) const {
        if (!str) return false;
        for (size_t i = 0; str[i] != '\0'; ++i) {
            if (str[i] != '0' && str[i] != '1') {
                return false;
            }
        }
        return true;
    }
};

int main() {

    String s1("Hello");
    String s2(" World");
    String s3 = s1 + s2;
    std::cout << "Concatenated String: " << s3.getLength() << " characters\n";

    BitString b1("1010");
    BitString b2("0101");
    BitString b3 = b1 + b2;
    std::cout << "BitString 1: " << (b1 == b2 ? "Equal" : "Not Equal") << "\n";

    b1.negate();
    std::cout << "Negated BitString: " << (b1 == b2 ? "Equal" : "Not Equal") << "\n";

    return 0;
}
