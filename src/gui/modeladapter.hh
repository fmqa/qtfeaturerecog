#ifndef GUI_MODELADAPTER_HH
#define GUI_MODELADAPTER_HH

#include "partials/edgeoptions.hh"
#include "partials/circleoptions.hh"
#include "partials/ellipseoptions.hh"
#include "../model/ellipse_detector.hh"

namespace mm {

    ellipse_detector& options_to_model(const EllipseOptions &, ellipse_detector &);
    EllipseOptions& model_to_options(const ellipse_detector &, EllipseOptions &);
}

#endif /* #ifndef GUI_MODELADAPTER_HH */
