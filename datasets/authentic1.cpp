#include <cstdio>
#include <cmath>

bool check_prime(unsigned int candidate) {
    if (candidate < 2) return false;
    if (candidate == 2 || candidate == 3) return true;
    if (candidate % 2 == 0 || candidate % 3 == 0) return false;

    unsigned int limit = static_cast<unsigned int>(std::sqrt(candidate));
    for (unsigned int d = 5; d <= limit; d += 6) {
        if (candidate % d == 0 || candidate % (d + 2) == 0)
            return false;
    }
    return true;
}

int main() {
    const int target_count = 1000;
    int found = 0;
    unsigned int candidate = 2;
    bool first = true;
    while (found < target_count) {
        if (check_prime(candidate)) {
            if (!first) std::printf(" ");
            std::printf("%u", candidate);
            ++found;
            first = false;
        }
        ++candidate;
    }
    std::printf("\n");
    return 0;
}