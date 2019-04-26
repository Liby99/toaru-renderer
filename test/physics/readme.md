# Previously Working Parameters

## Cube System V1

``` c++
k = std::make_unique<MaterialTensor>(3200000.0f, 0.499f);
d = std::make_unique<MaterialTensor>(160000.0f, 150000.0f, false);
mat = std::make_unique<PhysicsMaterial>(1000.f, *k, *d);
```

## Jelly System

``` c++
k = make_unique<MaterialTensor>(2000000.0f, 0.499f);
d = make_unique<MaterialTensor>(5000.0f, 1000.0f, false);
mat = make_unique<PhysicsMaterial>(1000.f, *k, *d);
```