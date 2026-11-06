#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/004_Строки/001_Хэши/code.hpp"

using i128 = __int128;

ll norm(i128 value, ll mod) {
    ll result = (ll)(value % mod);
    if (result < 0) {
        result += mod;
    }
    return result;
}

template <class Iterable> std::vector<i128> raw_values(const Iterable &values) {
    std::vector<i128> raw;
    for (auto value : values) {
        using Value = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<Value, char> || std::is_same_v<Value, unsigned char>) {
            raw.push_back(static_cast<unsigned char>(value));
        } else {
            raw.push_back(static_cast<i128>(value));
        }
    }
    return raw;
}

ll code_of(i128 value, ll mod) { return norm(value + 1, mod); }

ll roll(const std::vector<i128> &raw, int left, int right, ll mod) {
    ll base = Hash::BASE % mod;
    ll hash = 0;
    for (int i = left; i < right; ++i) {
        hash = norm((i128)hash * base + code_of(raw[i], mod), mod);
    }
    return hash;
}

template <class Iterable> void expect_hash(const Iterable &values, const vll &mods) {
    Hash hash(values, mods);
    std::vector<i128> raw = raw_values(values);
    int n = (int)raw.size();
    ASSERT_EQ(hash.n, n);
    ASSERT_EQ(hash.mods, mods);
    ASSERT_EQ((int)hash.pref.size(), (int)mods.size());
    ASSERT_EQ((int)hash.rev.size(), (int)mods.size());
    ASSERT_EQ((int)hash.pw.size(), (int)mods.size());

    std::vector<i128> reversed(raw.rbegin(), raw.rend());
    for (int j = 0; j < (int)mods.size(); ++j) {
        ll mod = mods[j];
        ll base = Hash::BASE % mod;
        ll power = 1;
        ll forward = 0;
        ll backward = 0;
        ASSERT_EQ((int)hash.pref[j].size(), n + 1);
        ASSERT_EQ((int)hash.rev[j].size(), n + 1);
        ASSERT_EQ((int)hash.pw[j].size(), n + 1);
        for (int i = 0; i <= n; ++i) {
            EXPECT_EQ(hash.pw[j][i], power) << j << " " << i;
            EXPECT_EQ(hash.pref[j][i], forward) << j << " " << i;
            EXPECT_EQ(hash.rev[j][i], backward) << j << " " << i;
            if (i == n) {
                break;
            }
            power = norm((i128)power * base, mod);
            forward = norm((i128)forward * base + code_of(raw[i], mod), mod);
            backward = norm((i128)backward * base + code_of(reversed[i], mod), mod);
        }
    }

    auto expect_range = [&](int left, int right) {
        vll forward(mods.size());
        vll backward(mods.size());
        for (int j = 0; j < (int)mods.size(); ++j) {
            forward[j] = roll(raw, left, right, mods[j]);
            backward[j] = roll(reversed, left, right, mods[j]);
        }
        EXPECT_EQ(hash.get(left, right), forward) << left << " " << right;
        EXPECT_EQ(hash.get(left, right, true), backward) << left << " " << right;
        for (int j = 0; j < (int)mods.size(); ++j) {
            EXPECT_EQ(hash.get_one(j, left, right), forward[j]);
            EXPECT_EQ(hash.get_one(j, left, right, true), backward[j]);
        }
    };

    if (n <= 24) {
        for (int left = 0; left <= n; ++left) {
            for (int right = left; right <= n; ++right) {
                expect_range(left, right);
            }
        }
    } else {
        for (int right = 0; right <= n; ++right) {
            expect_range(0, right);
            expect_range(right, n);
        }
    }

    if (mods.empty() || n > 12) {
        return;
    }
    for (int l1 = 0; l1 <= n; ++l1) {
        for (int r1 = l1; r1 <= n; ++r1) {
            for (int l2 = 0; l2 <= n; ++l2) {
                for (int r2 = l2; r2 <= n; ++r2) {
                    for (int mask = 0; mask < 4; ++mask) {
                        bool rev1 = mask & 1;
                        bool rev2 = mask & 2;
                        bool same = r1 - l1 == r2 - l2;
                        for (int j = 0; j < (int)mods.size() && same; ++j) {
                            const std::vector<i128> &first = rev1 ? reversed : raw;
                            const std::vector<i128> &second = rev2 ? reversed : raw;
                            same = roll(first, l1, r1, mods[j]) == roll(second, l2, r2, mods[j]);
                        }
                        EXPECT_EQ(hash.equal(l1, r1, l2, r2, rev1, rev2), same)
                            << l1 << " " << r1 << " " << l2 << " " << r2 << " " << mask;
                    }
                }
            }
        }
    }
}

}  // namespace

TEST(a_004_Stroki_001_Heshi, EmptyStringAndEmptyModuli) {
    vll mods{1000000007LL};
    Hash empty_text(std::string{}, mods);
    EXPECT_EQ(empty_text.n, 0);
    EXPECT_EQ(empty_text.get(0, 0), vll{0});
    EXPECT_TRUE(empty_text.equal(0, 0, 0, 0));

    Hash no_mods(std::string{"ABC"}, vll{});
    EXPECT_EQ(no_mods.n, 3);
    EXPECT_TRUE(no_mods.get(0, 3).empty());
    EXPECT_TRUE(no_mods.equal(0, 2, 1, 3));
    EXPECT_FALSE(no_mods.equal(0, 1, 0, 2));
}

TEST(a_004_Stroki_001_Heshi, HandCheckedValues) {
    vll mods{1000000007LL};
    Hash one(std::string{"A"}, mods);
    EXPECT_EQ(one.get(0, 1), vll{66});
    EXPECT_EQ(one.get(0, 0), vll{0});
    EXPECT_EQ(one.get(1, 1), vll{0});
    EXPECT_EQ(one.pw[0][0], 1);
    EXPECT_EQ(one.pw[0][1], Hash::BASE % mods[0]);

    Hash two(std::string{"AB"}, mods);
    EXPECT_EQ(two.get(0, 2), vll{151232965});
    EXPECT_EQ(two.get(0, 1), vll{66});
    EXPECT_EQ(two.get(1, 2), vll{67});

    Hash negative(std::vector<int>{-5}, mods);
    EXPECT_EQ(negative.get(0, 1), vll{1000000003});

    Hash bytes(std::vector<unsigned char>{0, 255}, mods);
    EXPECT_EQ(bytes.get(0, 1), vll{1});
    EXPECT_EQ(bytes.get(1, 2), vll{256});

    std::string high(1, static_cast<char>(0xFF));
    EXPECT_EQ(Hash(high, mods).get(0, 1), vll{256});
    EXPECT_EQ(Hash(std::vector<int>{-1}, mods).get(0, 1), vll{0});
}

TEST(a_004_Stroki_001_Heshi, HalfOpenRangesAndEqualContent) {
    vll mods{1000000007LL, 1000000009LL};
    std::string text = "abcabc";
    Hash hash(text, mods);
    EXPECT_EQ(hash.get(0, 3), hash.get(3, 6));
    EXPECT_TRUE(hash.equal(0, 3, 3, 6));
    EXPECT_FALSE(hash.equal(0, 3, 1, 4));
    EXPECT_FALSE(hash.equal(0, 2, 0, 3));
    EXPECT_TRUE(hash.equal(2, 2, 5, 5));
    EXPECT_NE(hash.get(0, 2), hash.get(1, 3));
}

TEST(a_004_Stroki_001_Heshi, ReversedStringAndPalindromes) {
    vll mods{1000000007LL, 1000000009LL};
    std::string text = "abXba";
    std::string flipped(text.rbegin(), text.rend());
    Hash forward(text, mods);
    Hash backward(flipped, mods);
    EXPECT_EQ(forward.get(0, (int)text.size(), true), backward.get(0, (int)flipped.size()));
    EXPECT_TRUE(forward.equal(0, 5, 0, 5, false, true));
    EXPECT_TRUE(forward.equal(1, 4, 1, 4, false, true));
    EXPECT_FALSE(forward.equal(0, 2, 3, 5, false, true));

    std::string different = "abcde";
    Hash other(different, mods);
    EXPECT_FALSE(other.equal(0, 5, 0, 5, false, true));
    EXPECT_TRUE(other.equal(1, 4, 1, 4));
    EXPECT_FALSE(other.equal(1, 4, 1, 4, false, true));
}

TEST(a_004_Stroki_001_Heshi, BaseCollapsesWhenModulusDividesIt) {
    vll mods{Hash::BASE};
    std::string text = "hash";
    Hash hash(text, mods);
    for (int right = 1; right <= (int)text.size(); ++right) {
        for (int left = 0; left < right; ++left) {
            unsigned char last = static_cast<unsigned char>(text[right - 1]);
            EXPECT_EQ(hash.get(left, right)[0], (last + 1) % Hash::BASE);
        }
    }
}

TEST(a_004_Stroki_001_Heshi, ModulusOneMakesEveryHashZero) {
    vll mods{1};
    Hash hash(std::string{"ABC"}, mods);
    EXPECT_EQ(hash.get(0, 3), vll{0});
    EXPECT_EQ(hash.get(1, 2), vll{0});
    EXPECT_TRUE(hash.equal(0, 2, 1, 3));
}

TEST(a_004_Stroki_001_Heshi, StringsIntegersAndSourceIsCopied) {
    vll mods{97, 1000000007LL};
    expect_hash(std::string{"A"}, mods);
    expect_hash(std::string{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"}, mods);
    expect_hash(std::string{"aaaa"}, mods);
    expect_hash(std::string{"abacaba"}, mods);
    expect_hash(std::string(1, static_cast<char>(0xFF)), mods);
    expect_hash(std::vector<int>{1, 2, 3, 2, 1}, mods);
    expect_hash(std::vector<int>{-5, 0, 8, -5}, mods);
    expect_hash(std::vector<ll>{LLONG_MIN, -1, 0, LLONG_MAX}, mods);

    std::string text = "mutable";
    Hash hash(text, mods);
    text[0] = 'X';
    expect_hash(std::string{"mutable"}, mods);
    EXPECT_EQ(hash.get(0, 7), Hash(std::string{"mutable"}, mods).get(0, 7));

    vll source{97};
    Hash from_mods(std::string{"ab"}, source);
    source[0] = 1;
    EXPECT_EQ(from_mods.mods, vll{97});
}

TEST(a_004_Stroki_001_Heshi, RandomSmallStrings) {
    std::mt19937 rng(2202);
    std::uniform_int_distribution<int> letter(0, 25);
    vll mods{1000000007LL, 1000000009LL, 97};
    for (int n : {0, 1, 2, 3, 5, 8, 12}) {
        std::string text(n, 'a');
        for (char &ch : text) {
            ch = (char)('a' + letter(rng));
        }
        expect_hash(text, mods);
    }
}

TEST(a_004_Stroki_001_Heshi, LoadAndSampledRanges) {
    std::mt19937 rng(11);
    std::uniform_int_distribution<int> byte(0, 255);
    const int n = 100000;
    std::string text(n, '\0');
    for (char &ch : text) {
        ch = (char)byte(rng);
    }
    vll mods{1000000007LL, 1000000009LL};
    Hash hash(text, mods);
    std::vector<i128> raw = raw_values(text);
    for (int j = 0; j < (int)mods.size(); ++j) {
        ll mod = mods[j];
        ll base = Hash::BASE % mod;
        ll forward = 0;
        for (int i = 0; i < n; ++i) {
            forward = norm((i128)forward * base + code_of(raw[i], mod), mod);
            ASSERT_EQ(hash.pref[j][i + 1], forward);
        }
    }
    std::uniform_int_distribution<int> index(0, n);
    for (int sample = 0; sample < 200; ++sample) {
        int left = index(rng);
        int right = index(rng);
        if (left > right) {
            std::swap(left, right);
        }
        for (int j = 0; j < (int)mods.size(); ++j) {
            EXPECT_EQ(hash.get_one(j, left, right), roll(raw, left, right, mods[j]));
        }
    }
}
