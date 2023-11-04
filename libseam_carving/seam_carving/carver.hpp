//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_CARVER_HPP
#define SEAM_CARVING_CARVER_HPP

// project
#include <seam_carving/energy.hpp>
#include <seam_carving/seam.hpp>

// 3rd party
#include <opencv2/core.hpp>

// std
#include <algorithm>
#include <limits>
#include <vector>

namespace seam_carving {
    /**
     * Base class for all carvers
     */
    class Carver {
    protected:
        /**
         * @param img the image to find the optimal seam in
         * @returns the optimal seam to be removed
         */
        static Seam FindVerticalSeam(cv::InputArray img);
        static Seam FindHorizontalSeam(cv::InputArray img);

        /**
 * @param seam the seam to be removed
 * @param img the target image
 */
        static void RemoveVerticalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);
        static void RemoveHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);

        /**
         * @param seam the seam to be inserted
         * @param img the target image
         */
        static void InsertVerticalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);
        static void InsertHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);

    public:
        /**
         * Finds the first k vertical/horizontal seams for removal.
         *
         * @param k the number of seams to find
         * @param input the image to find the seams in
         * @return a vector of the first k seams for removal
         */
        static std::vector<Seam> FindVerticalSeams(const int& k, cv::InputArray input);
        static std::vector<Seam> FindHorizontalSeams(const int& k, cv::InputArray input);

        /**
         * Inserts k vertical/horizontal seams.
         * @param k the number of seams to insert
         * @param input the image to find seams in
         * @param output the image to insert seams into
         */
        static void InsertVerticalSeams(const int& k, cv::InputArray input, cv::OutputArray output);
        static void InsertHorizontalSeams(const int& k, cv::InputArray input, cv::OutputArray output);

        /**
         * Inserts the specified seams, regardless of direction, into the image appropriately.
         * @param seams the seams to be inserted
         * @param output the image to insert seams into
         */
        static void InsertSeams(const std::vector<Seam>& seams, cv::InputOutputArray output);

        /**
         * Removes k vertical/horizontal seams.
         * @param k the number of seams to remove
         * @param input the image to find seams in
         * @param output the image to remove seams from
         */
        static void RemoveVerticalSeams(const int& k, cv::InputArray input, cv::OutputArray output);
        static void RemoveHorizontalSeams(const int& k, cv::InputArray input, cv::OutputArray output);

        /**
         * Removes the specified seams, regardless of direction, from the image appropriately.
         * @param seams the seams to be removed
         * @param output the image to remove seams from
         */
        static void RemoveSeams(const std::vector<Seam>& seams, cv::InputOutputArray output);

        /**
         * @param rows the target number of rows for the new image
         * @param cols the target number of cols for the new image
         * @param input the image to be carved
         * @param output a carved version of input
         */
        static void Carve(const int& rows, const int& cols, cv::InputArray input, cv::OutputArray output);
    };
}

#endif //SEAM_CARVING_CARVER_HPP
