

template<unsigned int N>
struct fibbo
{
    enum { value = N * fibbo<N-1>::value };
};

template<>
struct fibbo<1>
{
    enum { value = 1 };
};


int result = fibbo<5>::value;

int main(){}
