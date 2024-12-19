#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#pragma once

class SpatialPartition {
private:
    int cellSize;
    int width, height;
    std::vector<std::vector<std::vector<const sf::FloatRect*>>> grid;

public:
    SpatialPartition(int cellSize, int width, int height)
        : cellSize(cellSize), width(width), height(height) {
        int cols = (width + cellSize - 1) / cellSize;
        int rows = (height + cellSize - 1) / cellSize;
        grid.resize(rows, std::vector<std::vector<const sf::FloatRect*>>(cols));
    }

    void clear() {
        for (auto& row : grid)
            for (auto& cell : row)
                cell.clear();
    }

    void insert(const sf::FloatRect* bounds) {
    
        float centerX = bounds->left + bounds->width / 2;
        float centerY = bounds->top + bounds->height / 2;

        
        int left = std::max(0, static_cast<int>((centerX - bounds->width / 2) / cellSize));
        int top = std::max(0, static_cast<int>((centerY - bounds->height / 2) / cellSize));
        int right = std::min(width / cellSize - 1, static_cast<int>((centerX + bounds->width / 2) / cellSize));
        int bottom = std::min(height / cellSize - 1, static_cast<int>((centerY + bounds->height / 2) / cellSize));

     
        for (int row = top; row <= bottom; ++row) {
            for (int col = left; col <= right; ++col) {
                grid[row][col].push_back(bounds);
            }
        }
    }

    std::vector<const sf::FloatRect*> query(const sf::FloatRect& area) const {
       
        float centerX = area.left + area.width / 2;
        float centerY = area.top + area.height / 2;

        int left = std::max(0, static_cast<int>((centerX - area.width / 2) / cellSize));
        int top = std::max(0, static_cast<int>((centerY - area.height / 2) / cellSize));
        int right = std::min(width / cellSize - 1, static_cast<int>((centerX + area.width / 2) / cellSize));
        int bottom = std::min(height / cellSize - 1, static_cast<int>((centerY + area.height / 2) / cellSize));

        std::vector<const sf::FloatRect*> result;

        for (int row = top; row <= bottom; ++row) {
            for (int col = left; col <= right; ++col) {
                result.insert(result.end(), grid[row][col].begin(), grid[row][col].end());
            }
        }

        return result;
    }
};
