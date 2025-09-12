using System;


class Test
{
    public int value = 0;
    public static Test operator ++(Test t)
    {
        t.value++;
        return t;
    }

    public void __iadd__(int a) {
        value += a;
    }
}

class Demo
{
    static void Main()
    {
        using (var calc = new Calculator())
        {
            Console.WriteLine(calc.Add(3, 4));  // 7
            Console.WriteLine(calc.Sub(10, 6)); // 4
        }

        using (var v = new Vector2D(3, 4))
        {
            Console.WriteLine(v.Length2()); // 25
        }

        using (Cat c = new Cat())
        {
            Console.WriteLine($"Cat weight: {c.weight()}");

            Animal a = c.copy();
            Console.WriteLine($"Animal weight: {a.weight()}");
            Cat cc = a.As<Cat>();
            Console.WriteLine($"As<Cat> weight: {cc.weight()}");

            bool eq = (cc == a);
            Console.WriteLine($"a == cc ? {eq}");

            bool eq2 = (c == a);
            Console.WriteLine($"a == c ? {eq2}");

            Console.WriteLine(c.nativePtr.ToInt64().ToString());
            Console.WriteLine(a.nativePtr.ToInt64().ToString());
            Console.WriteLine(cc.nativePtr.ToInt64().ToString());

            Animal c2 = c.copy();
            Console.WriteLine($"c2 weight {c2.weight()}");
        }

        using (var views = new ViewList())
        {
            foreach (var (id, view) in views)
            {
                Console.WriteLine($"{id} => {view.Id}");
            }
        }

        Test t = new Test();
        t.__iadd__(1);

        using (var s = new StringObj())
        {
            s.set("C# set 你好");
            Console.WriteLine("c# StringObj set: " + s.get());
        }
    }
}
