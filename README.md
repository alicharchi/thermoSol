# Vapor-Liquid Equilibrium (VLE) Calculations Library

A C++ library for performing vapor-liquid equilibrium (VLE) calculations, based on the methods and algorithms presented in:

> Smith, J.M., Van Ness, H.C., & Abbott, M.M. (2005).  
> *Introduction to Chemical Engineering Thermodynamics* (7th Ed.).  
> McGraw-Hill, Chapter 14.

## 📘 Overview

This library provides numerical tools to compute phase equilibrium properties for binary and multicomponent mixtures, assuming ideal or non-ideal behavior. It includes common algorithms from classical chemical engineering thermodynamics such as:

- **Raoult’s Law** for ideal mixtures  
- **Modified Raoult’s Law** with activity coefficients  
- **Fugacity-based flash calculations**  
- **Bubble point and dew point calculations**  
- **Isothermal flash (Rachford-Rice) solution**  

This library is useful for educational purposes, simulation backends, and integrating into chemical process design tools.

---

## 📦 Features

- Supports **binary** and **multicomponent** systems
- Implements **bubble point**, **dew point**, and **isothermal flash** calculations
- Modular design for incorporating **activity coefficient models**
- Designed for performance and clarity in **C++**

---

## 🛠 Installation

### Requirements

- C++17 or higher
- CMake or Visual Studio(for building)


