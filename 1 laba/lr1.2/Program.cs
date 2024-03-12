using static System.Console;


var lock1 = new object();
var lock2 = new object();

void AcquireLocks1()
{
    var threadId = Thread.CurrentThread.ManagedThreadId;

    lock (lock1)
    {
        WriteLine($"Thread {threadId} acquired lock 1.");
        Thread.Sleep(1000);
        WriteLine($"Thread {threadId} attempted to acquire lock 2.");
        lock (lock2)
        {
            WriteLine($"Thread {threadId} acquired lock 2.");
        }
    }
}

void AcquireLocks2()
{
    var threadId = Thread.CurrentThread.ManagedThreadId;

    lock (lock2)
    {
        WriteLine($"Thread {threadId} acquired lock 2.");
        Thread.Sleep(1000);
        WriteLine($"Thread {threadId} attempted to acquire lock 1.");
        lock (lock1)
        {
            WriteLine($"Thread {threadId} acquired lock 1.");
        }
    }
}

var thread1 = new Thread(AcquireLocks1);
var thread2 = new Thread(AcquireLocks2);

thread1.Start();
thread2.Start();

thread1.Join();
thread2.Join();

WriteLine("Finished.");
ReadLine();

//using static System.Console;

//var lock1 = new object();
//var lock2 = new object();

//void AcquireLocks1()
//{
//    var threadId = Thread.CurrentThread.ManagedThreadId;

//    while (true)
//    {
//        if (Monitor.TryEnter(lock1, TimeSpan.FromSeconds(1)))
//        {
//            try
//            {
//                WriteLine($"Thread {threadId} acquired lock 1.");
//                Thread.Sleep(1000);
//                WriteLine($"Thread {threadId} attempted to acquire lock 2.");

//                if (Monitor.TryEnter(lock2, TimeSpan.FromSeconds(1)))
//                {
//                    try
//                    {
//                        WriteLine($"Thread {threadId} acquired lock 2.");
//                        break;
//                    }
//                    finally
//                    {
//                        Monitor.Exit(lock2);
//                    }
//                }
//            }
//            finally
//            {
//                Monitor.Exit(lock1);
//            }
//        }
//    }

//    WriteLine($"Thread {threadId} is done.");
//}

//void AcquireLocks2()
//{
//    var threadId = Thread.CurrentThread.ManagedThreadId;

//    while (true)
//    {
//        if (Monitor.TryEnter(lock2, TimeSpan.FromSeconds(1)))
//        {
//            try
//            {
//                WriteLine($"Thread {threadId} acquired lock 2.");
//                Thread.Sleep(1000);
//                WriteLine($"Thread {threadId} attempted to acquire lock 1.");

//                if (Monitor.TryEnter(lock1, TimeSpan.FromSeconds(1)))
//                {
//                    try
//                    {
//                        WriteLine($"Thread {threadId} acquired lock 1.");
//                        break;
//                    }
//                    finally
//                    {
//                        Monitor.Exit(lock1);
//                    }
//                }
//            }
//            finally
//            {
//                Monitor.Exit(lock2);
//            }
//        }
//    }

//    WriteLine($"Thread {threadId} is done.");
//}

//var thread1 = new Thread(AcquireLocks1);
//var thread2 = new Thread(AcquireLocks2);

//thread1.Start();
//thread2.Start();

//thread1.Join();
//thread2.Join();

//WriteLine("Finished.");
//ReadLine();