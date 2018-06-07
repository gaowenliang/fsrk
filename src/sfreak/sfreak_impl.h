#ifndef SFREAK_IMPL_H
#define SFREAK_IMPL_H

#include "sfreak.hpp"

#define SHOW_IMG

namespace cv
{

#define FREAK_LOG2 0.693147180559945
#define N_ORIENTATION 180 // 256
#define N_POINTS 43
#define SMALLEST_KP_SIZE 7 // smallest size of keypoints
#define N_SCALES SFREAK::NB_SCALES
#define N_PAIRS SFREAK::NB_PAIRS
#define N_ORIENPAIRS SFREAK::NB_ORIENPAIRS

struct DescriptionPair
{
    uchar i; // index of the first point
    uchar j; // index of the second point
};

struct OrientationPair
{
    uchar i;       // index of the first point
    uchar j;       // index of the second point
    int weight_dx; // dx/(norm_sq))*4096
    int weight_dy; // dy/(norm_sq))*4096
};

// used to sort pairs during pairs selection
struct PairStat
{
    double mean;
    int idx;
};

struct sortMean
{
    bool operator( )( const PairStat& a, const PairStat& b ) const
    {
        return a.mean < b.mean;
    }
};

/*!
 SFREAK implementation
 */
class SFREAK_Impl : public SFREAK
{
    public:
    /** Constructor
     * @param orientationNormalized enable orientation normalization
     * @param scaleNormalized enable scale normalization
     * @param patternScale scaling of the description pattern
     * @param nbOctave number of octaves covered by the detected keypoints
     * @param selectedPairs (optional) user defined selected pairs
     */
    explicit SFREAK_Impl( bool orientationNormalized              = true,
                          bool scaleNormalized                    = true,
                          float patternScale                      = 22.0f,
                          int nOctaves                            = 4,
                          const std::vector< int >& selectedPairs = std::vector< int >( ) );

    explicit SFREAK_Impl( std::string cam_file,
                          bool orientationNormalized              = true,
                          bool scaleNormalized                    = true,
                          float patternScale                      = 22.0f,
                          int nOctaves                            = 4,
                          const std::vector< int >& selectedPairs = std::vector< int >( ) );

    virtual ~SFREAK_Impl( );

    /** returns the descriptor length in bytes */
    virtual int descriptorSize( ) const;

    /** returns the descriptor type */
    virtual int descriptorType( ) const;

    /** returns the default norm type */
    virtual int defaultNorm( ) const;

    /** select the 512 "best description pairs"
     * @param images grayscale images set
     * @param keypoints set of detected keypoints
     * @param corrThresh correlation threshold
     * @param verbose print construction information
     * @return list of best pair indexes
     */
    std::vector< int > selectPairs( const std::vector< Mat >& images,
                                    std::vector< std::vector< KeyPoint > >& keypoints,
                                    const double corrThresh = 0.7,
                                    bool verbose            = true );
    virtual void compute( InputArray image, std::vector< KeyPoint >& keypoints, OutputArray descriptors );
    virtual void buildPatternTable( );
    virtual bool savePatternTableToData( std::string filename );
    virtual bool loadPatternTableFromData( std::string filename );

    //    void drawPattern( );

    protected:
    void buildPattern( );
    bool loadPattern( );

    template< typename imgType, typename iiType >
    imgType meanIntensity( InputArray image,
                           const float kp_x,
                           const float kp_y,
                           const unsigned int scale,
                           const unsigned int rot,
                           const unsigned int point );

    template< typename imgType, typename iiType >
    imgType meanIntensityByTable( cv::Mat image,
                                  const float theta,
                                  const float cosTheta,
                                  const float sinTheta,
                                  const float kp_x,
                                  const float kp_y,
                                  const cv::Vec5f* pParam,
                                  const unsigned int point );

    template< typename srcMatType, typename iiMatType >
    void computeDescriptors( InputArray image, std::vector< KeyPoint >& keypoints, OutputArray descriptors );

    template< typename srcMatType >
    void extractDescriptor( srcMatType* pointsValue, void** ptr );

    private:
    float disOfPoints( cv::Point2f pt0, cv::Point2f pt1 );
    float sinAngOfPoints( cv::Point2f pt0, cv::Point2f pt1, float dis );
    float cosAngOfPoints( cv::Point2f pt0, cv::Point2f pt1, float dis );
    float angOfPoints( cv::Point2f pt0, cv::Point2f pt1 );
    cv::Point2f rotatePoint( const float x, const float y, const float cosA, const float sinA );

    bool orientationNormalized; // true if the orientation is normalized, false otherwise
    bool scaleNormalized;       // true if the scale is normalized, false otherwise
    double patternScale;        // scaling of the pattern
    int nOctaves;               // number of octaves
    bool extAll; // true if all pairs need to be extracted for pairs selection

    double patternScale0;
    int nOctaves0;
    std::vector< int > selectedPairs0;

    // look-up table for the pattern points
    // (position+sigma of all points at all scales and orientation)
    cv::Mat m_patternTable;

    int patternSizes[NB_SCALES]; // size of the pattern at a specific scale (used to check
                                 // if a point is within image boundaries)
    DescriptionPair descriptionPairs[NB_PAIRS];
    OrientationPair orientationPairs[NB_ORIENPAIRS];
};
}
#endif // SFREAK_IMPL_H
