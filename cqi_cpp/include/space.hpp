template<typename T, typename U>
class Space {
    public:
        T shape;

        Space(T);

        virtual U sample() = 0;

        virtual bool contains(U) = 0;
};
