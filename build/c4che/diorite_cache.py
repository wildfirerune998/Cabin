AR = 'arm-none-eabi-ar'
ARFLAGS = 'rcs'
AS = 'arm-none-eabi-gcc'
BINDIR = '/usr/local/bin'
BLOCK_MESSAGE_KEYS = []
BUILD_DIR = 'diorite'
BUILD_TYPE = 'app'
BUNDLE_BIN_DIR = 'diorite'
BUNDLE_NAME = 'Cabin.pbw'
CC = ['arm-none-eabi-gcc']
CCLNK_SRC_F = []
CCLNK_TGT_F = ['-o']
CC_NAME = 'gcc'
CC_SRC_F = []
CC_TGT_F = ['-c', '-o']
CC_VERSION = ('4', '7', '2')
CFLAGS = ['-std=c99', '-mcpu=cortex-m3', '-mthumb', '-ffunction-sections', '-fdata-sections', '-g', '-fPIE', '-Os', '-D_TIME_H_', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter', '-Wno-error=unused-function', '-Wno-error=unused-variable']
CFLAGS_MACBUNDLE = ['-fPIC']
CFLAGS_cshlib = ['-fPIC']
CPPPATH_ST = '-I%s'
DEFINES = ['RELEASE', 'PBL_PLATFORM_DIORITE', 'PBL_BW', 'PBL_RECT', 'PBL_MICROPHONE', 'PBL_HEALTH', 'PBL_SMARTSTRAP', 'PBL_DISPLAY_WIDTH=144', 'PBL_DISPLAY_HEIGHT=168', 'PBL_SDK_3']
DEFINES_ST = '-D%s'
DEST_BINFMT = 'elf'
DEST_CPU = 'arm'
DEST_OS = 'linux'
INCLUDES = ['diorite']
LD = 'arm-none-eabi-ld'
LIBDIR = '/usr/local/lib'
LIBPATH_ST = '-L%s'
LIB_DIR = 'node_modules'
LIB_JSON = [{u'_location': u'/pebble-clay', u'_spec': u'pebble-clay@^1.0.4', u'_integrity': u'sha1-/fkvD9x3CpecBodOqiRXzC52I0Q=', u'keywords': [u'pebble', u'config', u'configuration', u'pebble-package'], u'devDependencies': {u'chai': u'^3.4.1', u'mocha': u'^2.3.4', u'through': u'^2.3.8', u'gulp-inline': u'0.0.15', u'karma-source-map-support': u'^1.1.0', u'deepcopy': u'^0.6.1', u'eslint-plugin-standard': u'^1.3.1', u'stringify': u'^3.2.0', u'gulp-insert': u'^0.5.0', u'gulp': u'^3.9.0', u'gulp-htmlmin': u'^1.3.0', u'deamdify': u'^0.2.0', u'bourbon': u'^4.2.6', u'eslint-config-pebble': u'^1.2.0', u'eslint': u'^1.5.1', u'karma-coverage': u'^0.5.3', u'watchify': u'^3.7.0', u'require-from-string': u'^1.1.0', u'gulp-sourcemaps': u'^1.6.0', u'karma-mocha': u'^0.2.1', u'sinon': u'^1.17.3', u'joi': u'^6.10.1', u'browserify': u'^13.0.0', u'sassify': u'^0.9.1', u'gulp-autoprefixer': u'^3.1.0', u'karma-mocha-reporter': u'^1.1.5', u'autoprefixer': u'^6.3.1', u'browserify-istanbul': u'^0.2.1', u'karma-threshold-reporter': u'^0.1.15', u'gulp-sass': u'^2.1.1', u'vinyl-source-stream': u'^1.1.0', u'gulp-uglify': u'^1.5.2', u'karma-chrome-launcher': u'^0.2.2', u'vinyl-buffer': u'^1.0.0', u'del': u'^2.0.2', u'karma': u'^0.13.19', u'karma-browserify': u'^5.0.1', u'tosource': u'^1.0.0', u'postcss': u'^5.0.14'}, u'_from': u'pebble-clay@^1.0.4', u'pebble': {u'sdkVersion': u'3', u'capabilities': [u'configurable'], u'projectType': u'package', u'targetPlatforms': [u'aplite', u'basalt', u'chalk', u'diorite', u'emery'], u'resources': {u'media': []}}, u'bundleDependencies': False, u'_phantomChildren': {}, u'version': u'1.0.4', u'_resolved': u'https://registry.npmjs.org/pebble-clay/-/pebble-clay-1.0.4.tgz', u'_inBundle': False, u'homepage': u'https://github.com/pebble/clay#readme', u'_requested': {u'name': u'pebble-clay', u'escapedName': u'pebble-clay', u'saveSpec': None, u'rawSpec': u'^1.0.4', u'raw': u'pebble-clay@^1.0.4', u'registry': True, u'fetchSpec': u'^1.0.4', u'type': u'range'}, u'description': u'Pebble Config Framework', u'repository': {u'url': u'git+https://github.com/pebble/clay.git', u'type': u'git'}, u'_requiredBy': [u'/'], u'dependencies': {}, u'scripts': {u'pebble-publish': u'npm run pebble-clean && npm run build && pebble build && pebble package publish && npm run pebble-clean', u'test-travis': u'gulp && ./node_modules/.bin/karma start ./test/karma.conf.js --single-run --browsers chromeTravisCI && ./node_modules/.bin/eslint ./', u'pebble-build': u'npm run build && pebble build', u'test-debug': u'(export DEBUG=true && ./node_modules/.bin/gulp && ./node_modules/.bin/karma start ./test/karma.conf.js --no-single-run)', u'lint': u'eslint ./', u'dev': u'gulp dev', u'build': u'gulp', u'test': u'gulp && ./node_modules/.bin/karma start ./test/karma.conf.js --single-run', u'pebble-clean': u'rm -rf tmp src/js/index.js && pebble clean'}, 'path': 'node_modules/pebble-clay/dist', u'name': u'pebble-clay', u'license': u'MIT', u'deprecated': False, u'author': {u'name': u'Pebble Technology'}, u'bugs': {u'url': u'https://github.com/pebble/clay/issues'}, 'js_paths': ['node_modules/pebble-clay/dist/js/index.js'], u'_where': u'/home/Cabin', u'_id': u'pebble-clay@1.0.4', u'_shasum': u'fdf92f0fdc770a979c06874eaa2457cc2e762344'}]
LIB_RESOURCES_JSON = {u'pebble-clay': []}
LIB_ST = '-l%s'
LINKFLAGS = ['-mcpu=cortex-m3', '-mthumb', '-Wl,--gc-sections', '-Wl,--warn-common', '-fPIE', '-Os']
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_cshlib = ['-shared']
LINKFLAGS_cstlib = ['-Wl,-Bstatic']
LINK_CC = ['arm-none-eabi-gcc']
MESSAGE_KEYS = {u'TEMPERATURE': 0, u'WEATHER': 7, u'METRIC': 5, u'API': 4, u'SUNSET': 3, u'READY': 6, u'CONDITIONS': 1, u'SUNRISE': 2}
MESSAGE_KEYS_DEFINITION = '/home/pebble/pebble-projects/Cabin/Cabin/build/src/message_keys.auto.c'
MESSAGE_KEYS_HEADER = '/home/pebble/pebble-projects/Cabin/Cabin/build/include/message_keys.auto.h'
MESSAGE_KEYS_JSON = '/home/pebble/pebble-projects/Cabin/Cabin/build/js/message_keys.json'
NODE_PATH = '/home/pebble/.pebble-sdk/SDKs/current/node_modules'
PEBBLE_SDK_COMMON = '/home/pebble/.pebble-sdk/SDKs/current/sdk-core/pebble/common'
PEBBLE_SDK_PLATFORM = '/home/pebble/.pebble-sdk/SDKs/current/sdk-core/pebble/diorite'
PEBBLE_SDK_ROOT = '/home/pebble/.pebble-sdk/SDKs/current/sdk-core/pebble'
PLATFORM = {'TAGS': ['diorite', 'bw', 'rect', 'mic', 'strap', 'health', '144w', '168h'], 'MAX_FONT_GLYPH_SIZE': 256, 'ADDITIONAL_TEXT_LINES_FOR_PEBBLE_H': [], 'MAX_APP_BINARY_SIZE': 65536, 'MAX_RESOURCES_SIZE': 1048576, 'MAX_APP_MEMORY_SIZE': 65536, 'MAX_WORKER_MEMORY_SIZE': 10240, 'NAME': 'diorite', 'BUNDLE_BIN_DIR': 'diorite', 'BUILD_DIR': 'diorite', 'MAX_RESOURCES_SIZE_APPSTORE': 262144, 'DEFINES': ['PBL_PLATFORM_DIORITE', 'PBL_BW', 'PBL_RECT', 'PBL_MICROPHONE', 'PBL_HEALTH', 'PBL_SMARTSTRAP', 'PBL_DISPLAY_WIDTH=144', 'PBL_DISPLAY_HEIGHT=168']}
PLATFORM_NAME = 'diorite'
PREFIX = '/usr/local'
PROJECT_INFO = {'appKeys': {u'TEMPERATURE': 0, u'WEATHER': 7, u'METRIC': 5, u'API': 4, u'SUNSET': 3, u'READY': 6, u'CONDITIONS': 1, u'SUNRISE': 2}, u'watchapp': {u'watchface': True}, u'displayName': u'Cabin', u'uuid': u'218ead1b-6d45-46eb-9dd5-651e78aa6d70', u'messageKeys': {u'TEMPERATURE': 0, u'WEATHER': 7, u'METRIC': 5, u'API': 4, u'SUNSET': 3, u'READY': 6, u'CONDITIONS': 1, u'SUNRISE': 2}, 'companyName': u'Hel', u'enableMultiJS': True, u'sdkVersion': u'3', u'capabilities': [u'location', u'configurable'], 'versionLabel': u'3.0', u'targetPlatforms': [u'aplite', u'basalt', u'diorite', u'chalk'], 'longName': u'Cabin', 'shortName': u'Cabin', u'resources': {u'media': [{u'type': u'bitmap', u'name': u'LOADING', u'file': u'images/loading.png'}, {u'type': u'bitmap', u'name': u'BASIC_BG', u'file': u'images/basic_bg.png'}, {u'type': u'bitmap', u'name': u'NIGHT', u'file': u'images/night.png'}, {u'type': u'bitmap', u'name': u'CLOUDSNIGHT', u'file': u'images/clouds-night.png'}, {u'type': u'bitmap', u'name': u'DAY', u'file': u'images/day.png'}, {u'type': u'bitmap', u'name': u'RAIN', u'file': u'images/rain.png'}, {u'type': u'bitmap', u'name': u'SNOW', u'file': u'images/snow.png'}, {u'type': u'bitmap', u'name': u'CLOUDS', u'file': u'images/clouds.png'}, {u'type': u'bitmap', u'name': u'CLOUDS_BW', u'file': u'images/clouds_bw.png'}, {u'type': u'bitmap', u'name': u'BASIC_BG_ROUND', u'file': u'images/basic_bg_round.png'}, {u'type': u'bitmap', u'name': u'LOADING_ROUND', u'file': u'images/loading_round.png'}, {u'type': u'bitmap', u'name': u'NIGHT_ROUND', u'file': u'images/night_round.png'}, {u'type': u'bitmap', u'name': u'NIGHT_BW', u'file': u'images/night_round.png'}, {u'type': u'bitmap', u'name': u'CLOUDSNIGHT_ROUND', u'file': u'images/clouds-night_round.png'}, {u'type': u'bitmap', u'name': u'CLOUDSNIGHT_BW', u'file': u'images/clouds-night_bw.png'}, {u'type': u'bitmap', u'name': u'DAY_ROUND', u'file': u'images/day_round.png'}, {u'type': u'bitmap', u'name': u'DAY_BW', u'file': u'images/day_round.png'}, {u'type': u'bitmap', u'name': u'RAIN_ROUND', u'file': u'images/rain_round.png'}, {u'type': u'bitmap', u'name': u'SNOW_ROUND', u'file': u'images/snow_round.png'}, {u'type': u'bitmap', u'name': u'SNOW_BW', u'file': u'images/snow_round.png'}, {u'type': u'bitmap', u'name': u'CLOUDS_ROUND', u'file': u'images/clouds_round.png'}, {u'type': u'bitmap', u'name': u'THUNDERSTORMS', u'file': u'images/thunderstorms.png'}, {u'type': u'bitmap', u'name': u'THUNDERSTORMS_ROUND', u'file': u'images/thunderstorms_round.png'}, {u'type': u'bitmap', u'name': u'TORNADO', u'file': u'images/tornado.png'}, {u'type': u'bitmap', u'name': u'TORNADO_ROUND', u'file': u'images/tornado_round.png'}, {u'type': u'bitmap', u'name': u'HAZE', u'file': u'images/haze.png'}, {u'type': u'bitmap', u'name': u'HAZE_ROUND', u'file': u'images/haze_round.png'}, {u'type': u'bitmap', u'name': u'HAZE_BW', u'file': u'images/haze_bw.png'}]}, 'name': u'Cabin'}
REQUESTED_PLATFORMS = [u'aplite', u'basalt', u'diorite', u'chalk']
RESOURCES_JSON = [{u'type': u'bitmap', u'name': u'LOADING', u'file': u'images/loading.png'}, {u'type': u'bitmap', u'name': u'BASIC_BG', u'file': u'images/basic_bg.png'}, {u'type': u'bitmap', u'name': u'NIGHT', u'file': u'images/night.png'}, {u'type': u'bitmap', u'name': u'CLOUDSNIGHT', u'file': u'images/clouds-night.png'}, {u'type': u'bitmap', u'name': u'DAY', u'file': u'images/day.png'}, {u'type': u'bitmap', u'name': u'RAIN', u'file': u'images/rain.png'}, {u'type': u'bitmap', u'name': u'SNOW', u'file': u'images/snow.png'}, {u'type': u'bitmap', u'name': u'CLOUDS', u'file': u'images/clouds.png'}, {u'type': u'bitmap', u'name': u'CLOUDS_BW', u'file': u'images/clouds_bw.png'}, {u'type': u'bitmap', u'name': u'BASIC_BG_ROUND', u'file': u'images/basic_bg_round.png'}, {u'type': u'bitmap', u'name': u'LOADING_ROUND', u'file': u'images/loading_round.png'}, {u'type': u'bitmap', u'name': u'NIGHT_ROUND', u'file': u'images/night_round.png'}, {u'type': u'bitmap', u'name': u'NIGHT_BW', u'file': u'images/night_round.png'}, {u'type': u'bitmap', u'name': u'CLOUDSNIGHT_ROUND', u'file': u'images/clouds-night_round.png'}, {u'type': u'bitmap', u'name': u'CLOUDSNIGHT_BW', u'file': u'images/clouds-night_bw.png'}, {u'type': u'bitmap', u'name': u'DAY_ROUND', u'file': u'images/day_round.png'}, {u'type': u'bitmap', u'name': u'DAY_BW', u'file': u'images/day_round.png'}, {u'type': u'bitmap', u'name': u'RAIN_ROUND', u'file': u'images/rain_round.png'}, {u'type': u'bitmap', u'name': u'SNOW_ROUND', u'file': u'images/snow_round.png'}, {u'type': u'bitmap', u'name': u'SNOW_BW', u'file': u'images/snow_round.png'}, {u'type': u'bitmap', u'name': u'CLOUDS_ROUND', u'file': u'images/clouds_round.png'}, {u'type': u'bitmap', u'name': u'THUNDERSTORMS', u'file': u'images/thunderstorms.png'}, {u'type': u'bitmap', u'name': u'THUNDERSTORMS_ROUND', u'file': u'images/thunderstorms_round.png'}, {u'type': u'bitmap', u'name': u'TORNADO', u'file': u'images/tornado.png'}, {u'type': u'bitmap', u'name': u'TORNADO_ROUND', u'file': u'images/tornado_round.png'}, {u'type': u'bitmap', u'name': u'HAZE', u'file': u'images/haze.png'}, {u'type': u'bitmap', u'name': u'HAZE_ROUND', u'file': u'images/haze_round.png'}, {u'type': u'bitmap', u'name': u'HAZE_BW', u'file': u'images/haze_bw.png'}]
RPATH_ST = '-Wl,-rpath,%s'
SANDBOX = False
SDK_VERSION_MAJOR = 5
SDK_VERSION_MINOR = 86
SHLIB_MARKER = None
SIZE = 'arm-none-eabi-size'
SONAME_ST = '-Wl,-h,%s'
STLIBPATH_ST = '-L%s'
STLIB_MARKER = None
STLIB_ST = '-l%s'
SUPPORTED_PLATFORMS = ['chalk', 'emery', 'basalt', 'diorite', 'aplite']
TARGET_PLATFORMS = ['diorite', 'chalk', 'basalt', 'aplite']
TIMESTAMP = 1682642242
USE_GROUPS = True
VERBOSE = 0
WEBPACK = '/home/pebble/.pebble-sdk/SDKs/current/node_modules/.bin/webpack'
cprogram_PATTERN = '%s'
cshlib_PATTERN = 'lib%s.so'
cstlib_PATTERN = 'lib%s.a'
macbundle_PATTERN = '%s.bundle'
