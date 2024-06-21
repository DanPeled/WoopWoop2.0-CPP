#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <vector>
#include <algorithm>

namespace wpwp
{
    /**
     * @brief A signal class for handling function callbacks with a variable number of arguments.
     *
     * @tparam R The types of arguments for the signal.
     */
    template <typename... R>
    struct Signal
    {
    public:
        /**
         * @brief Invokes all connected functions with the provided arguments.
         *
         * @param args The arguments to pass to the connected functions.
         */
        void invoke(R... args)
        {
            for (auto &f : m_functions)
            {
                f(std::forward<R>(args)...);
            }
        }

        /**
         * @brief Binds a function to the signal.
         *
         * @param f The function to bind.
         */
        void bind(std::function<void(R...)> &&f)
        {
            m_functions.emplace_back(std::forward<std::function<void(R...)>>(f));
        }

        /**
         * @brief Unbinds a function from the signal.
         *
         * @param f The function to unbind.
         */
        void unbind(std::function<void(R...)> &&f)
        {
            auto it = std::remove_if(m_functions.begin(), m_functions.end(),
                                     [&](const std::function<void(R...)> &func)
                                     {
                                         return f.target_type() == func.target_type() && *f.template target<void(R...)>() == *func.template target<void(R...)>();
                                     });

            m_functions.erase(it, m_functions.end());
        }

        /**
         * @brief Combines the functions of two signals into one.
         *
         * This function takes another signal and combines its functions with the
         * current signal. After the operation, the current signal will contain
         * all functions from both signals.
         *
         * @param s1 The first signal.
         * @param s2 The second signal to combine with.
         */
        static void combine(Signal<R...> &s1, const Signal<R...> &s2)
        {
            for (auto &f : s2.getBindedFunctions())
            {
                s1.bind(std::move(f));
            }
        }

        /**
         * @brief Overloaded += operator for binding a function or combining signals.
         *
         * @param f The function to bind or signal to combine.
         * @return Reference to the modified Signal object.
         */
        Signal &operator+=(std::function<void(R...)> &&f)
        {
            bind(std::forward<std::function<void(R...)>>(f));
            return *this;
        }

        /**
         * @brief Overloaded += operator for combining signals.
         *
         * This operator combines the functions of another signal with the current one.
         *
         * @param other The signal to combine with.
         * @return Reference to the modified Signal object.
         */
        Signal &operator+=(const Signal<R...> &other)
        {
            combine(*this, other);
            return *this;
        }

        /**
         * @brief Overloaded -= operator for unbinding a function from the signal.
         *
         * @param f The function to unbind.
         * @return Reference to the modified Signal object.
         */
        Signal &operator-=(std::function<void(R...)> &&f)
        {
            unbind(std::forward<std::function<void(R...)>>(f));
            return *this;
        }

        /**
         * @brief Get all of the binded functions of the signal.
         *
         * @return A vector containing all of the binded functions.
         */
        std::vector<std::function<void(R...)>> getBindedFunctions() const
        {
            return m_functions;
        }

    private:
        std::vector<std::function<void(R...)>> m_functions{}; // Vector to store connected functions.
    };
} // namespace wpwp

#endif // SIGNAL_HPP
