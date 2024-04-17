#include <iostream>
#include <cstdint>
#include <bitset>

union FloatToBits {
    float f;
    uint32_t u;
};

void printFloatBits(float num) {
    FloatToBits converter;
    converter.f = num;
    std::bitset<32> bits(converter.u);
    std::cout << bits << std::endl;
}


float multiplyFloats(float num1, float num2) {

    FloatToBits converter1, converter2;
    converter1.f = num1;
    converter2.f = num2;


    bool sign1 = (converter1.u >> 31) & 1;
    bool sign2 = (converter2.u >> 31) & 1;
    int exponent1 = ((converter1.u >> 23) & 0xFF) - 127;
    int exponent2 = ((converter2.u >> 23) & 0xFF) - 127;
    uint32_t significand1 = (converter1.u & 0x7FFFFF) | 0x800000;
    uint32_t significand2 = (converter2.u & 0x7FFFFF) | 0x800000;

    if (exponent1 == -127 || exponent2 == -127) {
        return 0.0f;
    }


    int newExponent = exponent1 + exponent2;


    if (newExponent > 127) {
        std::cerr << "Overflow al sumar los exponentes." << std::endl;
        return 0.0f;
    } else if (newExponent < -126) {
        std::cerr << "Underflow al sumar los exponentes." << std::endl;
        return 0.0f;
    }

   
    uint64_t resultSignificand = (uint64_t)significand1 * significand2;


    bool carry = resultSignificand & 0x80000000000000;
    uint32_t resultExponent = newExponent + (carry ? 1 : 0);
    uint32_t resultSignificandNormalized = resultSignificand >> 23;

 
    uint32_t result = (sign1 ^ sign2) << 31 | (resultExponent + 127) << 23 | (resultSignificandNormalized & 0x7FFFFF);
    FloatToBits resultConverter;
    resultConverter.u = result;
    return resultConverter.f;
}

int main() {
    float num1, num2;
    std::cout << "Ingrese el primer número en punto flotante: ";
    std::cin >> num1;
    std::cout << "Ingrese el segundo número en punto flotante: ";
    std::cin >> num2;

    std::cout << "Resultado real de la multiplicación: " << num1 * num2 << std::endl;

    std::cout << "Bits del primer número en punto flotante: ";
    printFloatBits(num1);
    std::cout << "Bits del segundo número en punto flotante: ";
    printFloatBits(num2);
    std::cout << "Bits del resultado emulado en punto flotante: ";
    printFloatBits(multiplyFloats(num1, num2));
    std::cout << "Bits del resultado real en punto flotante: ";
    printFloatBits(num1 * num2);

    return 0;
}