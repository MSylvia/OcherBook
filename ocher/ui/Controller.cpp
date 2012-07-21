#include "mxml.h"

#include "clc/storage/File.h"
#include "clc/support/Logger.h"

#include "ocher/ui/Factory.h"
#include "ocher/ui/Controller.h"
#include "ocher/settings/Options.h"

// TODO:  replace all this hardcoded stuff with factory:
#include "ocher/fmt/epub/Epub.h"
#include "ocher/fmt/epub/LayoutEpub.h"
#include "ocher/fmt/text/Text.h"
#include "ocher/fmt/text/LayoutText.h"


Controller::Controller(UiFactory *factory) :
    m_factory(factory)
{
}

void Controller::run()
{
    Browse& browser = m_factory->getBrowser();

    // TODO:  workflow

    browser.browse();

    clc::Buffer memLayout;

    // TODO:  probe file type
    // TODO:  complete hardcoded hack to test with here...
    clc::File f(opt.file);
    char buf[2];
    if (f.read(buf, 2) != 2 || buf[0] != 'P' || buf[1] != 'K') {
        Text text(opt.file);
        LayoutText layout(&text);
        clc::Log::info("ocher", "Loading %s: %s", text.getFormatName().c_str(), opt.file);
        memLayout = layout.unlock();
    } else {
        Epub epub(opt.file);
        LayoutEpub layout(&epub);

        clc::Log::info("ocher", "Loading %s: %s", epub.getFormatName().c_str(), opt.file);

        clc::Buffer html;
        for (int i = 0; ; i++) {
            if (epub.getSpineItemByIndex(i, html) != 0)
                break;
            mxml_node_t *tree = epub.parseXml(html);
            if (tree) {
                layout.append(tree);
                mxmlDelete(tree);
            }
        }
        memLayout = layout.unlock();
    }

    Renderer& renderer = m_factory->getRenderer();
    renderer.init(memLayout);
    renderer.render(1);
}
