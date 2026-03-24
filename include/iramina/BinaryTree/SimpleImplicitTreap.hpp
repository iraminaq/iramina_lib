#pragma once
#include "iramina/BinaryTree/ImplicitTreap.hpp"
#include <type_traits>

template <class Traits>
struct SimpleImplicitTreapHelper {
    using T = typename Traits::T;
    using S = T; // SimpleImplicitTreap では T = S を前提とする

    // F型の抽出 (use_lazy が定義されていなくても安全に取得するため SFINAE を使用)
    template <class U, class = void> struct extract_F { using type = T; };
    template <class U> struct extract_F<U, std::void_t<typename U::F>> { using type = typename U::F; };
    using F = typename extract_F<Traits>::type;

    // use_lazy の静的判定
    template <class U, class = void> struct get_use_lazy : std::false_type {};
    template <class U> struct get_use_lazy<U, std::void_t<decltype(U::use_lazy)>> { static constexpr bool value = U::use_lazy; };
    static constexpr bool use_lazy = get_use_lazy<Traits>::value;

    // use_reverse の静的判定
    template <class U, class = void> struct get_use_reverse : std::false_type {};
    template <class U> struct get_use_reverse<U, std::void_t<decltype(U::use_reverse)>> { static constexpr bool value = U::use_reverse; };
    static constexpr bool use_reverse = get_use_reverse<Traits>::value;

    // ImplicitTreap のシグネチャに合わせたブリッジ関数
    static S op_fn(const S& a, const S& b) {
        return Traits::op(a, b);
    }

    static S e_fn() {
        return Traits::e();
    }

    static S lift_fn(const T& x) {
        return x; // T = S
    }

    static T mapping_point_fn(const F& f, const T& x) {
        if constexpr (use_lazy) {
            return Traits::apply(f, x, 1);
        } else {
            return x;
        }
    }

    static S mapping_fn(const F& f, const S& x, int len) {
        if constexpr (use_lazy) {
            return Traits::apply(f, x, len);
        } else {
            return x;
        }
    }

    static F composition_fn(const F& f, const F& g) {
        if constexpr (use_lazy) {
            return Traits::composition(f, g);
        } else {
            return F{}; // ダミー
        }
    }

    static F id_fn() {
        if constexpr (use_lazy) {
            return Traits::id();
        } else {
            return F{}; // ダミー
        }
    }

    static S reverse_prod_fn(const S& x) {
        if constexpr (use_reverse) {
            return Traits::reverse_prod(x);
        } else {
            return x; // デフォルトは恒等写像
        }
    }

    using Type = ImplicitTreap<
        T, S, F,
        op_fn, e_fn, lift_fn,
        mapping_point_fn, mapping_fn, composition_fn, id_fn, reverse_prod_fn
    >;
};

// ユーザーが利用するためのエイリアステンプレート
template <class Traits>
using SimpleImplicitTreap = typename SimpleImplicitTreapHelper<Traits>::Type;
