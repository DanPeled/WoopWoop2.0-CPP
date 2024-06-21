#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>
#include <sstream>
#include <vector>
#include <memory>
#include <functional>

namespace wpwp
{
    class Engine;

    /**
     * @brief Utility class providing various helper functions.
     */
    class Util
    {
    public:
        /**
         * @brief Generates a UUID version 4.
         *
         * @return A string representing the generated UUID.
         */
        static std::string generate_uuid_v4();

        /**
         * @brief Gets the delta time.
         *
         * @return A float representing the delta time.
         */
        static float deltaTime() { return m_deltaTime; }

    private:
        static std::random_device rd;                // Random device for generating seeds.
        static std::mt19937 gen;                     // Mersenne Twister random number generator.
        static std::uniform_int_distribution<> dis;  // Uniform distribution for integers.
        static std::uniform_int_distribution<> dis2; // Uniform distribution for integers.
        static float m_deltaTime;

        friend Engine;
    };

    /**
     * @brief Utility class for performing operations on vectors.
     */
    class VectorUtil
    {
    public:
        /**
         * @brief Transforms each element in a vector using a provided function.
         *
         * @tparam T Type of the elements in the input vector.
         * @tparam Func Type of the callable object to transform the elements.
         * @param v The input vector containing elements of type T.
         * @param func A callable object that takes an element of type T and returns a transformed element.
         * @return A vector containing the transformed elements.
         */
        template <typename T, typename Func>
        static auto select(const std::vector<T> &v, Func func) -> std::vector<decltype(func(std::declval<T>()))>
        {
            using R = decltype(func(std::declval<T>()));
            std::vector<R> results;
            forEach(v, [&](T i)
                    { results.push_back(func(i)); });
            return results;
        }

        template <typename T>
        static std::shared_ptr<T> firstOrDefault(const std::vector<std::shared_ptr<T>> &components,
                                                 std::function<bool(std::shared_ptr<T>)> predicate,
                                                 std::shared_ptr<T> defaultValue)
        {
            for (auto &c : components)
            {
                if (predicate(c))
                {
                    return std::dynamic_pointer_cast<T>(c);
                }
            }
            return defaultValue;
        }
        /**
         * @brief Applies a provided function to each element in a vector.
         *
         * @tparam T Type of the elements in the input vector.
         * @tparam Func Type of the callable object to apply to each element.
         * @param v The input vector containing elements of type T.
         * @param func A callable object that takes an element of type T.
         */
        template <typename T, typename Func>
        static void forEach(const std::vector<T> &v, Func func)
        {
            for (const auto &i : v)
            {
                func(i);
            }
        }

        template <typename T>
        static T firstOrDefault(const std::vector<T> &v, std::function<bool(T)> func, T defaultValue)
        {
            for (const auto &i : v)
            {
                if (func(i))
                    return i;
            }
            return defaultValue;
        };

        template <typename T, typename Predicate>
        static std::vector<T> filter(const std::vector<T> &v, Predicate func)
        {
            std::vector<T> results;

            for (const auto &i : v)
            {
                if (func(i))
                {
                    results.push_back(i);
                }
            }

            return results;
        }
    };
} // namespace wpwp

#endif // UTIL_HPP
