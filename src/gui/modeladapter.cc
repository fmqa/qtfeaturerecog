#include "modeladapter.hh"

mm::ellipse_detector& mm::options_to_model(const EllipseOptions &options, ellipse_detector &model) {
    return model.with_minimum_minor(options.minMinor())
                .with_maximum_minor(options.maxMinor())
                .with_minimum_major(options.minMajor())
                .with_maximum_major(options.maxMajor())
                .with_minimum_score(options.minScore())
                .with_maximum_score(options.maxScore());
}

mm::EllipseOptions& mm::model_to_options(const ellipse_detector &model, EllipseOptions &options) {
    options.setMinMinor(model.minimum_minor());
    options.setMaxMinor(model.maximum_minor());
    options.setMinMajor(model.minimum_major());
    options.setMaxMajor(model.maximum_major());
    options.setMinScore(model.minimum_score());
    options.setMaxScore(model.maximum_score());
    return options;
}
