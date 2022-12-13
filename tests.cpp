#include <iostream>
#include <gmpxx.h>
#include <cassert>
#include "PrimalityTests.h"

std::vector<mpz_class> to_test_big_primes = {
        mpz_class("1613817666714650887674415526206132421209233645959811711830916803563"),
        mpz_class("203910252718987041894944592737"),
        mpz_class("11111111111111111111111")
};
std::vector<mpz_class> to_test_big_nonprimes = {
        mpz_class(696969),
        mpz_class("11111111111111111111111111111111111111111111111111111"),
        mpz_class("2227732271974329183093218509617052533646483748405043211"),
};

void test_trivial() {
    std::cout << "Trivial primality test:" << std::endl;

    std::cout << "Looking for false negatives" << std::endl;
    std::vector<uint64_t> primes = { 2, 3, 5, 7, 11, 32143429 };
    for (auto prime : primes) {
        assert(PrimalityTests::trivial(prime));
    }

    std::cout << "Looking for false positives" << std::endl;
    std::vector<uint64_t> non_primes = { 1, 72347346, 782345783465, 4536897, 321434291, 617*619 };
    for (auto non_prime : non_primes) {
        assert(!PrimalityTests::trivial(non_prime));
    }
    std::cout << "Test sucessful!" << std::endl;
}

void test_fermat() {
    std::cout << "Fermat primality test:" << std::endl;
    gmp_randclass randomness(gmp_randinit_default);

    std::cout << "Looking for false negatives" << std::endl;
    for (const auto& prime : to_test_big_primes) {
        assert(PrimalityTests::fermat_test(randomness, prime, 20));
    }
    // boundary value
    assert(PrimalityTests::fermat_test(randomness, mpz_class(5), 20));

    std::cout << "Looking for false positives" << std::endl;
    for (const auto& non_prime : to_test_big_nonprimes) {
        assert(!PrimalityTests::fermat_test(randomness, non_prime, 20));
    }

    std::cout << "Test successful!" << std::endl;
}

void test_miller_rabin() {
    std::cout << "Fermat primality test: " << std::endl;
    gmp_randclass randomness(gmp_randinit_default);
    randomness.seed(0x1337);
    std::cout << "Starting test" << std::endl;

    std::cout << "Looking for false negatives" << std::endl;
    for (const auto& testval : to_test_big_primes) {
        assert(PrimalityTests::miller_rabin_test(randomness, testval, 20));
    }

    assert(PrimalityTests::miller_rabin_test(randomness, mpz_class(5), 20));

    std::cout << "Looking for false positives" << std::endl;
    for (const auto& testval : to_test_big_nonprimes) {
        assert(!PrimalityTests::miller_rabin_test(randomness, testval, 20));
    }
    std::cout << "Test successful!" << std::endl;
}

int main() {
    test_trivial();
    test_fermat();
    test_miller_rabin();
}
