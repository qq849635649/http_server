#pragma once

template<typename T>
class Singleton
{
private:
	struct object_creator
	{
		object_creator()
		{
            Singleton<T>::I();
        }
        inline void do_nothing() const {}
	};

	static object_creator create_object;
public:
    static T& I()
	{
		static T obj;
		create_object.do_nothing();
		return obj;
    }
};
template<typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;

