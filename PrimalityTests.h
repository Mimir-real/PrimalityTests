#ifndef PRIMALITY_PRIMALITYTESTS_H
#define PRIMALITY_PRIMALITYTESTS_H


namespace PrimalityTests {
    template<typename T>
    bool trivial(T n) {
        if (n <= 3){
            return n > 1;
        }

        if (!(n & 1)) {
            return false;
        }

        for (T i=3; i*i < n; i += 2) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }

    void prepare_fermat_witnesses(gmp_randclass &randgen, std::vector<mpz_class> &witnesses,
                                  const mpz_class &n, const unsigned int k) {
        witnesses.reserve(k);
        for (auto _i = 0; _i < k; _i++) {
            witnesses.emplace_back(randgen.get_z_range(n - 2 - 1) + 2);
        }
    }

    // assuming n > 4
    bool fermat_test(const mpz_class &n, const std::vector<mpz_class> &witnesses) {
        mpz_class n_minus_1 = n-1;
        for (auto witness : witnesses) {
            mpz_class res;
            auto rt = res.get_mpz_t();
            mpz_powm(rt, witness.get_mpz_t(), n_minus_1.get_mpz_t(), n.get_mpz_t());
            if (res != 1) {
                return false;
            }
        }
        return true;
    }

    bool fermat_test(gmp_randclass &randgen, const mpz_class &n, const unsigned int k = 40) {
        std::vector<mpz_class> witnesses;
        prepare_fermat_witnesses(randgen, witnesses, n, k);
        return fermat_test(n, witnesses);
    }

    // assuming n > 4
    bool miller_rabin_test(const mpz_class &n, const std::vector<mpz_class> &witnesses) {
        mpz_class d = n - 1;
        uint_least64_t s = 0;
        while (d % 2 == 0) {
            d /= 2;
            s++;
        }
        for (const auto &w: witnesses) {
            mpz_class x;
            auto xt = x.get_mpz_t();
            auto wt = w.get_mpz_t();
            auto dt = d.get_mpz_t();
            auto nt = n.get_mpz_t();
            mpz_powm(xt, wt, dt, nt);
            if (x == 1 || x == n - 1) {
                continue;
            }
            bool finish = true;
            for (uint_least64_t _i = 1; _i < s; _i++) { // s-1 times
                mpz_powm_ui(xt, xt, 2, nt);
                if (x == n - 1) {
                    finish = false;
                    break;
                }
            }
            if (finish) {
                return false;
            }
        }
        return true;
    }

    bool miller_rabin_test(gmp_randclass &randgen, const mpz_class &n, const unsigned int k = 20) {
        std::vector<mpz_class> witnesses;
        prepare_fermat_witnesses(randgen, witnesses, n, k);
        return miller_rabin_test(n, witnesses);
    }
}

#endif //PRIMALITY_PRIMALITYTESTS_H
