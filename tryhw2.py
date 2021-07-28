from uwimg import *

# TEST 1
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "results/dog-box7")

# TEST 2
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
thumb = nn_resize(blur, blur.w//7, blur.h//7)
save_image(thumb, "results/dogthumb")

# TEST 3 - HIGHPASS
im = load_image("data/dog.jpg")
highpass_filter = make_highpass_filter()
highpass = convolve_image(im, highpass_filter, 1)
save_image(highpass, "results/dog-highpass")

# TEST 4 - SHARPEN
im = load_image("data/dog.jpg")
sharpen_filter = make_sharpen_filter()
sharpen = convolve_image(im, sharpen_filter, 1)
save_image(sharpen, "results/dog-sharpen")

# TEST 5 - EMBOSS
im = load_image("data/dog.jpg")
emboss_filter = make_emboss_filter()
emboss = convolve_image(im, emboss_filter, 1)
save_image(emboss, "results/dog-emboss")