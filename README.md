# SFML C++ Paint Program
A high performance, interactive 2D painting application built with C++ and the Simple and Fast Multimedia Library (SFML). Features a persistent digital canvas, a custom cursor, and an asynchronous CLI for real-time adjustments.

## Features
* **Smooth Continous Drawing:** Uses distance-based linear interpolation formula to fill in the "empty" gaps created with fast mouse movements.
* **Asynchronous CLI:** A background thread listens for console commands to adjust brush size (1-200px) and colors.
* **Custom Pointer:** Replaces the standard OS cursor with an easily visible ConvexShape pointer. 

## Optimizations
* **O(1) Draw Calls:** Swapped `sf::VertexArray` (which causes exponential lag as it appends vertices) for a persistent `sf::RenderTexture` canvas. The GPU only draws one sprite per frame, ensuring a stable VSync frame rate regardless of stroke count.
* **Thread Saftey:** Utilized `std::atomic<int>` to safely share brush states between render loop and the input listener
