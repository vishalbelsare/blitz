
// floop12 generated by makeloops.py Wed Jun 29 17:04:04 2011

#include <blitz/vector2.h>
#include <blitz/array.h>
#include <random/uniform.h>
#include <blitz/benchext.h>

#ifdef BZ_HAVE_VALARRAY
 #define BENCHMARK_VALARRAY
#endif

#ifdef BENCHMARK_VALARRAY
#include <valarray>
#endif

BZ_NAMESPACE(blitz)
extern void sink();
BZ_NAMESPACE_END

BZ_USING_NAMESPACE(blitz)
BZ_USING_NAMESPACE(std)

#if defined(BZ_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES)
 #define floop12_f77 floop12_f77_
 #define floop12_f77overhead floop12_f77overhead_
 #define floop12_f90 floop12_f90_
 #define floop12_f90overhead floop12_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define floop12_f77 floop12_f77__
 #define floop12_f77overhead floop12_f77overhead__
 #define floop12_f90 floop12_f90__
 #define floop12_f90overhead floop12_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define floop12_f77 FLOOP12_F77
 #define floop12_f77overhead FLOOP12_F77OVERHEAD
 #define floop12_f90 FLOOP12_F90
 #define floop12_f90overhead FLOOP12_F90OVERHEAD
#endif

extern "C" {
  void floop12_f77(const int& N, float* y, float* x, float* a, float* b, float* c, float* d, const float& u);
  void floop12_f77overhead(const int& N, float* y, float* x, float* a, float* b, float* c, float* d, const float& u);
  void floop12_f90(const int& N, float* y, float* x, float* a, float* b, float* c, float* d, const float& u);
  void floop12_f90overhead(const int& N, float* y, float* x, float* a, float* b, float* c, float* d, const float& u);

}

void VectorVersion(BenchmarkExt<int>& bench, float u);
void ArrayVersion(BenchmarkExt<int>& bench, float u);
void ArrayVersion_unaligned(BenchmarkExt<int>& bench, float u);
void ArrayVersion_misaligned(BenchmarkExt<int>& bench, float u);
void ArrayVersion_index(BenchmarkExt<int>& bench, float u);
void doTinyVectorVersion(BenchmarkExt<int>& bench, float u);
void F77Version(BenchmarkExt<int>& bench, float u);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, float u);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, float u);
#endif

const int numSizes = 20;
const int Nmax=1<<(numSizes-1);
const int tvNmax=7;
const bool runvector=false; // no point as long as Vector is Array<1>

int main()
{
    int numBenchmarks = 6;
    if (runvector) numBenchmarks++;
#ifdef BENCHMARK_VALARRAY
    numBenchmarks++;
#endif
#ifdef FORTRAN_90
    numBenchmarks++;
#endif

    BenchmarkExt<int> bench("floop12: $y = u+$a; $x = $a+$b+$c+$d", numBenchmarks);

    bench.setNumParameters(numSizes);

    Array<int,1> parameters(numSizes);
    Array<long,1> iters(numSizes);
    Array<double,1> flops(numSizes);

    for (int i=0; i < numSizes; ++i)
    {
      parameters(i) = Nmax>>i;
      iters(i) = 50000000L / parameters(i);
	
        if (iters(i) < 2)
            iters(i) = 2;
        flops(i) = 4 * parameters(i);
    }

    bench.setParameterVector(parameters);
    bench.setIterations(iters);
    bench.setOpsPerIteration(flops);
    bench.setDependentVariable("flops");
    bench.beginBenchmarking();

    float u = 0.39123982498157938742;


    ArrayVersion(bench, u);
    ArrayVersion_unaligned(bench, u);
    ArrayVersion_misaligned(bench, u);
    ArrayVersion_index(bench, u);
    doTinyVectorVersion(bench, u);
    F77Version(bench, u);
#ifdef FORTRAN_90
    F90Version(bench, u);
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench, u);
#endif

    if(runvector)
      VectorVersion(bench, u);

    bench.endBenchmarking();

    bench.saveMatlabGraph("floop12.m");
    return 0;
}

template<class T>
void initializeRandomDouble(T* data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

template<class T>
void initializeRandomDouble(valarray<T>& data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

void VectorVersion(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Vector<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Vector<float> y(N);
        initializeRandomDouble(y.data(), N);
        Vector<float> x(N);
        initializeRandomDouble(x.data(), N);
        Vector<float> a(N);
        initializeRandomDouble(a.data(), N);
        Vector<float> b(N);
        initializeRandomDouble(b.data(), N);
        Vector<float> c(N);
        initializeRandomDouble(c.data(), N);
        Vector<float> d(N);
        initializeRandomDouble(d.data(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            y = u+a; x = a+b+c+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Array<T,1>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<float,1> y(N);
        initializeRandomDouble(y.dataFirst(), N);
        Array<float,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<float,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<float,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<float,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        Array<float,1> d(N);
        initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            y = u+a; x = a+b+c+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion_index(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Array<T,1> (indexexpr.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<float,1> y(N);
        initializeRandomDouble(y.dataFirst(), N);
        Array<float,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<float,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<float,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<float,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        Array<float,1> d(N);
        initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            y = u+a(tensor::i); x = a(tensor::i)+b(tensor::i)+c(tensor::i)+d(tensor::i);;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_unaligned(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Array<T,1> (unal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<float,1> yfill(N+1);
    Array<float,1> y(yfill(Range(1,N)));
    initializeRandomDouble(y.dataFirst(), N);

    Array<float,1> xfill(N+1);
    Array<float,1> x(xfill(Range(1,N)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<float,1> afill(N+1);
    Array<float,1> a(afill(Range(1,N)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<float,1> bfill(N+1);
    Array<float,1> b(bfill(Range(1,N)));
    initializeRandomDouble(b.dataFirst(), N);

    Array<float,1> cfill(N+1);
    Array<float,1> c(cfill(Range(1,N)));
    initializeRandomDouble(c.dataFirst(), N);

    Array<float,1> dfill(N+1);
    Array<float,1> d(dfill(Range(1,N)));
    initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            y = u+a; x = a+b+c+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_misaligned(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Array<T,1> (misal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<float,1> yfill(N+6);
    Array<float,1> y(yfill(Range(0,N+0-1)));
    initializeRandomDouble(y.dataFirst(), N);

    Array<float,1> xfill(N+6);
    Array<float,1> x(xfill(Range(1,N+1-1)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<float,1> afill(N+6);
    Array<float,1> a(afill(Range(2,N+2-1)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<float,1> bfill(N+6);
    Array<float,1> b(bfill(Range(3,N+3-1)));
    initializeRandomDouble(b.dataFirst(), N);

    Array<float,1> cfill(N+6);
    Array<float,1> c(cfill(Range(4,N+4-1)));
    initializeRandomDouble(c.dataFirst(), N);

    Array<float,1> dfill(N+6);
    Array<float,1> d(dfill(Range(5,N+5-1)));
    initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            y = u+a; x = a+b+c+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


template<int N>
void TinyVectorVersion(BenchmarkExt<int>& bench, float u)
{
        cout << bench.currentImplementation() << ": N = " << N << endl;

        const int sz = bench.getParameter();
        assert(N==sz);
                           
        long iters = bench.getIterations();

        TinyVector<float,N> y(N);
        initializeRandomDouble(y.dataFirst(), N);
        TinyVector<float,N> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        TinyVector<float,N> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        TinyVector<float,N> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        TinyVector<float,N> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        TinyVector<float,N> d(N);
        initializeRandomDouble(d.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            y = u+a; x = a+b+c+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}
        bench.stopOverhead();

	TinyVectorVersion<N>>1>(bench, u);
}

// end recursion
template<>
void TinyVectorVersion<0>(BenchmarkExt<int>& bench, float u)
{}

void doTinyVectorVersion(BenchmarkExt<int>& bench, float u)
{
  bench.beginImplementation("TinyVector<T>");
  // can't run tinyvector with full length because meta-unrolling
  // kills compiler...
  int N=Nmax;
  while(N> 1<<tvNmax) {
   bench.getParameter();
   bench.getIterations();
   bench.skip();
   N>>=1;
  }

  TinyVectorVersion< 1<<tvNmax >(bench, u);
  bench.endImplementation();
}


#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("valarray<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        long iters = bench.getIterations();

        valarray<float> y(N);
        initializeRandomDouble(y, N);
        valarray<float> x(N);
        initializeRandomDouble(x, N);
        valarray<float> a(N);
        initializeRandomDouble(a, N);
        valarray<float> b(N);
        initializeRandomDouble(b, N);
        valarray<float> c(N);
        initializeRandomDouble(c, N);
        valarray<float> d(N);
        initializeRandomDouble(d, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            y = u+a; x = a+b+c+d;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  sink();
	}
        bench.stopOverhead();
    }

    bench.endImplementation();
}
#endif

void F77Version(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Fortran 77");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        float* y = new float[N];
        initializeRandomDouble(y, N);
        float* x = new float[N];
        initializeRandomDouble(x, N);
        float* a = new float[N];
        initializeRandomDouble(a, N);
        float* b = new float[N];
        initializeRandomDouble(b, N);
        float* c = new float[N];
        initializeRandomDouble(c, N);
        float* d = new float[N];
        initializeRandomDouble(d, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop12_f77(N, y, x, a, b, c, d, u);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop12_f77overhead(N, y, x, a, b, c, d, u);

        bench.stopOverhead();

        delete [] y;
        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, float u)
{
    bench.beginImplementation("Fortran 90");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        float* y = new float[N];
        initializeRandomDouble(y, N);
        float* x = new float[N];
        initializeRandomDouble(x, N);
        float* a = new float[N];
        initializeRandomDouble(a, N);
        float* b = new float[N];
        initializeRandomDouble(b, N);
        float* c = new float[N];
        initializeRandomDouble(c, N);
        float* d = new float[N];
        initializeRandomDouble(d, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            floop12_f90(N, y, x, a, b, c, d, u);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            floop12_f90overhead(N, y, x, a, b, c, d, u);

        bench.stopOverhead();
        delete [] y;
        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;

    }

    bench.endImplementation();
}
#endif

