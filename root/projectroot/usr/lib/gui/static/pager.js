/*! pager.js - v1.1.0 - 2017-05-04
* http://oscar.finnsson.nu/pagerjs/
* Copyright (c) 2017 Oscar Finnsson; Licensed MIT */!function(a){var b=function(b,c){"use strict";var d=function(a,b){return function(){var d=arguments;return c.computed(function(){return a.apply(b,d)})}},e={};e.page=null,e.now=function(){return Date.now?Date.now():(new Date).valueOf()},e.extendWithPage=function(a){var b=new e.Page;a.$__page__=b,e.page=b,e.activePage$=d(e.getActivePage,e)()};var f=function(a,b,c){c=c||{},c.page=a,e[b].fire(c),a.val(b)&&a.val(b)(c)};b.each(["onBindingError","onSourceError","onNoMatch","onMatch","beforeRemove","afterRemove","beforeHide","afterHide","beforeShow","afterShow"],function(a,c){e[c]=b.Callbacks()}),e.showChild=function(a){var b=a&&1===a.length&&""===a[0]?[]:a;e.page.showPage(b)},e.getParentPage=function(a){for(;a;){if(a.$page&&"none"!==a.$page.val("urlToggle"))return a.$page;if(a.$data&&a.$data.$__page__)return a.$data.$__page__;a=a.$parentContext}return null};var g=null,h=null,i=function(a){h&&h.reject({cancel:!0}),g=null,a.substring(0,e.Href.hash.length)===e.Href.hash&&(a=a.slice(e.Href.hash.length));var b=j(a);e.showChild(b)};e.goTo=i,e.navigate=function(a){e.useHTML5history?e.Href5.history.pushState(null,null,a):location.hash=a};var j=function(a){return b.map(a.replace(/\+/g," ").split("/"),decodeURIComponent)},k={};k.value=c.utils.unwrapObservable,k.arrayValue=function(a){return b.map(a,function(a){return k.value(a)})};var l=function(a){for(var b,c={},d=/([^&=]+)=?([^&]*)/g;b=d.exec(a);)if(/\[\]$/.test(b[1])){var e=b[1].replace(/\[\]$/,"");e in c||(c[e]=[]),c[e].push(b[2])}else c[b[1]]=b[2];return c},m=function(a){if(!a)return{name:null,params:{}};var b=a.split("?"),c=b[0],d=b[1],e={};return d&&(e=l(d)),{name:c,params:e}};e.ChildManager=function(a,d){this.currentChildO=c.observable(null);var g=this;this.page=d,this.timeStamp=e.now(),this.hideChild=function(){var a=g.currentChildO();a&&(a.hidePage(function(){}),g.currentChildO(null))},this.showChild=function(c){var d=0===c.length;this.timeStamp=e.now();var h=this.timeStamp,i=g.currentChildO(),j=null,l=!1,n=m(c[0]),o=n.name,p=null;b.each(a(),function(a,b){if(!l){var c=b.getId();(e.ignoreRouteCase&&(c||"").toLowerCase()===(o||"").toLowerCase()||c===o||(""===o||null==o)&&b.isStartPage())&&(l=!0,j=b),"?"===c&&(p=b)}});for(var q=!1,r=g,s=function(a,b){if(!l){var c=b.getId(),d=b.getValue().modal;d&&((c===o||(""===o||null==o)&&b.isStartPage())&&(l=!0,j=b,q=!0),"?"!==c||p||(p=b,q=!0))}};!j&&r.page.parentPage&&!r.page.getValue().modal;){var t=r.page.parentPage.children;b.each(t(),s),j||(r=r.page.parentPage.childManager)}j||!p||d||(j=p),g.currentChildO(j),j&&(q?j.currentParentPage(g.page):j.currentParentPage(null));var u=function(){f(g.page,"onNoMatch",{route:c})},v=function(){f(g.page,"onMatch",{route:c});var a=0,d=k.value(j.getValue().guard);d&&b.isArray(d)?b.each(d,function(b,d){d(j,c,function(){a++,a===d.length-1&&g.timeStamp===h&&j.showPage(c.slice(1),n,c[0])},i)}):d?d(j,c,function(){g.timeStamp===h&&j.showPage(c.slice(1),n,c[0])},i):j.showPage(c.slice(1),n,c[0])};i&&i===j?v():i?i.hidePage(function(){j?v():u()}):j?v():u()}},e.Page=function(a,b,d,f,g){this.element=a,this.valueAccessor=b,this.allBindingsAccessor=d,this.viewModel=f,this.bindingContext=g,this.children=c.observableArray([]),this.childManager=new e.ChildManager(this.children,this),this.parentPage=null,this.currentId=null,this.getCurrentId=c.observable(),this.ctx=null,this.currentParentPage=c.observable(null),this.isVisible=c.observable(!1),this.originalRoute=c.observable(null),this.route=null};var n=e.Page.prototype;n.val=function(a){return k.value(this.getValue()[a])},n.currentChildPage=function(){return this.childManager.currentChildO},n.find=function(a){var c=k.value(a),d=this;if("/"===c.substring(0,1))d=e.page,c=c.slice(1);else for(;"../"===c.substring(0,3);)d=d.currentParentPage&&d.currentParentPage()?d.currentParentPage():d.parentPage,c=c.slice(3);var f=j(c);return b.each(f,function(a,b){d=d.child(b)()}),d},n.find$=function(a){return d(this.find,this)(a)};var o=function(a){return e.useHTML5history?b("base").attr("href")+a:e.Href.hash+a};n.path=function(a){var c=this,d=k.value(a);if(d&&"object"==typeof d&&d.path&&d.params&&!(d instanceof e.Page)){var f=d.path,g=d.params;return c.path(f)+"?"+b.param(g)}var h;if(null==d||""===d)h=c;else{if(!(d instanceof e.Page)){if("/"===d.substring(0,1)){var i=e.page.getFullRoute()().join("/")+d.substring(1);return o(i)}for(var j=0;"../"===d.substring(0,3);)j++,d=d.slice(3);var l=c.getFullRoute()(),m=l.slice(0,l.length-j).join("/"),n=(""===m?"":m+"/")+d;return o(n)}h=d}return o(h.getFullRoute()().join("/"))},n.path$=function(a){return d(this.path,this)(a)},n.async=function(a,b,c,d){var f=this;return function(){h&&h.reject({cancel:!0});var i=a();h=i,d&&d(i.state());var j=Math.random();g=j,i.done(function(){d&&d(i.state()),j===g&&e.navigate(f.path(b))}),i.fail(function(a){d&&d(i.state());var b=a&&a.cancel;j===g&&!b&&c&&e.navigate(f.path(c))})}},n.showPage=function(a,b,c){var d=this,e=d.currentId,f=d.pageRoute?d.pageRoute.params:null,g=d.isVisible();d.currentId=b?b.name||"":"",d.getCurrentId(d.currentId),d.isVisible(!0),c&&d.originalRoute(c),d.route=a,d.pageRoute=b,g?("?"===d.getId()&&e!==d.currentId&&d.show(),b&&f!==b.params&&d.setParams()):(d.setParams(),d.show()),d.childManager.showChild(a)},n.setParams=function(){if(this.pageRoute&&this.pageRoute.params){var a=this.pageRoute.params,d=this.ctx,e=this.val("params")||{};b.isArray(e)?b.each(e,function(b,e){var f=a[e];d[e]?d[e](f):d[e]=c.observable(f)}):b.each(e,function(b,e){var f,g=a[b];f=null==g?k.value(e):g,d[b]?d[b](f):d[b]=c.observable(f)})}if(this.pageRoute){var f=this.getValue().nameParam;f&&("string"==typeof f?this.ctx[f]?this.ctx[f](this.currentId):this.ctx[f]=c.observable(this.currentId):f(this.currentId))}},n.hidePage=function(a){var b=this;"show"!==b.val("urlToggle")?(b.hideElementWrapper(a),b.childManager.hideChild()):a&&a()};var p=function(b){try{c.applyBindingsToDescendants(b.childBindingContext,b.element)}catch(d){e.onBindingError.has()||a.console&&a.console.error&&a.console.error(d),f(b,"onBindingError",{error:d})}};n.init=function(){var a=this;a.cleanElement=a.element.innerHTML;var d=a.val("urlToggle"),e=a.val("id");"?"!==e&&a.getCurrentId(e);var g=c.utils.domData.get(a.element,"__ko_pagerjsBindingData");if(g)return{controlsDescendantBindings:!0};c.utils.domData.set(a.element,"__ko_pagerjsBindingData",a),c.utils.domNodeDisposal.addDisposeCallback(a.element,function(){f(a,"beforeRemove"),a.parentPage&&a.parentPage.children.remove(a),f(a,"afterRemove")});var h=a.getValue();if("none"!==d&&(a.parentPage=a.getParentPage(),a.parentPage.children.push(this),a.hideElement()),a.val("source")&&a.loadSource(a.val("source")),a.ctx=null,h.withOnShow)a.ctx={},a.childBindingContext=a.bindingContext.createChildContext(a.ctx),c.utils.extend(a.childBindingContext,{$page:this});else{var i=h["with"]||a.viewModel;if(a.ctx=k.value(i),a.augmentContext(),c.isObservable(i)){var j=c.observable(a.ctx);a.childBindingContext=a.bindingContext.createChildContext(j),c.utils.extend(a.childBindingContext,{$page:this}),p(a),i.subscribe(function(){j(k.value(i))})}else a.childBindingContext=a.bindingContext.createChildContext(a.ctx),c.utils.extend(a.childBindingContext,{$page:this,$observableData:void 0}),p(a)}if("none"!==d){var l=a.parentPage;l.route&&(l.route[0]===a.getId()||l.route.length&&"?"===a.getId())&&setTimeout(function(){l.showPage(l.route)},0)}else{var m=function(){b(a.element).is(":visible")&&a.showPage([])};setTimeout(m,0),a.getParentPage().isVisible.subscribe(function(a){a&&setTimeout(m,0)})}var n=a.getValue().bind;return c.isObservable(n)&&n(a),{controlsDescendantBindings:!0}},n.augmentContext=function(){var a=this,d=a.val("params");d&&(b.isArray(d)?b.each(d,function(b,d){a.ctx[d]||(a.ctx[d]=c.observable())}):b.each(d,function(b,e){a.ctx[b]||(c.isObservable(e)?a.ctx[b]=e:null===e?(d[b]=c.observable(null),a.ctx[b]=c.observable(null)):a.ctx[b]=c.observable(e))})),this.val("vars")&&b.each(this.val("vars"),function(b,d){c.isObservable(d)?a.ctx[b]=d:a.ctx[b]=c.observable(d)});var e=this.getValue().nameParam;e&&"string"==typeof e&&!a.ctx[e]&&(a.ctx[e]=c.observable(null)),this.setParams()},n.getValue=function(){return this.valueAccessor?k.value(this.valueAccessor()):{}},n.getParentPage=function(){return e.getParentPage(this.bindingContext)},n.getId=function(){return this.val("id")},n.id=function(){var a=this.getCurrentId();return null==a||""===a?this.getId():a},n.sourceUrl=function(a){var b=this;return"?"===this.getId()?c.computed(function(){var c;return c=b.val("deep")?[b.currentId].concat(b.route).join("/"):b.currentId,k.value(a).replace("{1}",c)}):c.computed(function(){return k.value(a)})},n.loadWithOnShow=function(a){var d=this;d.withOnShowLoaded&&d.val("sourceCache")===!0?d.showElementWrapper(a):(d.val("withOnShow")(function(e){!d.val("sourceOnShow")&&d.withOnShowLoaded&&(c.cleanNode(b(d.element)),b(d.element).empty(),d.element.innerHTML=d.cleanElement);var f=d.bindingContext.createChildContext(e);d.ctx=e,d.childBindingContext=f,d.augmentContext(),c.utils.extend(f,{$page:d}),p(d),d.showElementWrapper(a),d.route&&d.childManager.showChild(d.route)},d),d.withOnShowLoaded=!0)},n.loadSource=function(a,d){var f=this.getValue(),g=this,h=this.element,i=null,j=f.loader||e.loader;if(g.val("withOnShow")||g.showElementWrapper(d),"iframe"===f.frame){var l=b("iframe",b(h));0===l.length&&(l=b("<iframe></iframe>"),b(h).append(l)),j&&(i=k.value(j)(g,l),i.load()),l.one("load",function(){i&&i.unload(),f.sourceLoaded&&f.sourceLoaded(g)}),c.applyBindingsToNode(l[0],{attr:{src:this.sourceUrl(a)}})}else{j&&(i=k.value(j)(g,g.element),i.load());var m=function(){i&&i.unload(),g.val("withOnShow")?g.val("withOnShow")&&g.loadWithOnShow(d):p(g),f.sourceLoaded&&f.sourceLoaded(g),g.route&&g.childManager.showChild(g.route)};if("string"==typeof k.value(a)){var n=k.value(this.sourceUrl(a));r(h,n,function(){m()},g)}else{var o=b(h).children();k.value(a)(this,function(){b.each(o,function(a,b){c.utils.domNodeDisposal.removeNode(b)}),m()})}}};var q=/<script\b[^<]*(?:(?!<\/script>)<[^<]*)*<\/script>/gi,r=function(a,d,e,g){var h,i,j=b(a),k=d.indexOf(" ");k>=0&&(h=d.slice(k,d.length),d=d.slice(0,k));var l=jQuery.ajax({url:d,type:"GET",dataType:"html",complete:function(a,b){e&&j.each(e,i||[a.responseText,b,a])}}).done(function(a){i=arguments,b.each(j.children(),function(a,b){c.utils.domNodeDisposal.removeNode(b)}),j.html(h?jQuery("<div>").append(a.replace(q,"")).find(h):a)});return l.fail(function(){f(g,"onSourceError",{url:d,xhrPromise:l})}),j};n.show=function(b){var c=this.element,d=this;d.val("title")&&(a.document.title=d.val("title")),d.val("sourceOnShow")?!d.val("sourceCache")||!c.__pagerLoaded__||"number"==typeof d.val("sourceCache")&&c.__pagerLoaded__+1e3*d.val("sourceCache")<e.now()?(c.__pagerLoaded__=e.now(),d.loadSource(d.val("sourceOnShow"),b)):d.showElementWrapper(b):d.val("withOnShow")?d.loadWithOnShow(b):d.showElementWrapper(b)},n.titleOrId=function(){return this.val("title")||this.id()},n.showElementWrapper=function(a){var b=this;f(b,"beforeShow"),b.showElement(a),b.val("scrollToTop")&&b.element.scrollIntoView(),f(b,"afterShow")},n.showElement=function(a){this.val("showElement")?this.val("showElement")(this,a):this.val("fx")?e.fx[this.val("fx")].showElement(this,a):e.showElement?e.showElement(this,a):b(this.element).show(a)},n.hideElementWrapper=function(a){this.isVisible(!1),f(this,"beforeHide"),this.hideElement(a),f(this,"afterHide")},n.hideElement=function(a){this.val("hideElement")?this.val("hideElement")(this,a):this.val("fx")?e.fx[this.val("fx")].hideElement(this,a):e.hideElement?e.hideElement(this,a):(b(this.element).hide(),a&&a())},n.getFullRoute=function(){return this._fullRoute?this._fullRoute:(this._fullRoute=c.computed(function(){var a=null;return this.currentParentPage&&this.currentParentPage()?(a=this.currentParentPage().getFullRoute()().slice(0),a.push(this.originalRoute()||this.getId()),a):this.parentPage?(a=this.parentPage.getFullRoute()().slice(0),a.push(this.originalRoute()||this.getId()),a):[]},this),this._fullRoute)},n.getRole=function(){return this.val("role")||"next"},n.isStartPage=function(){return e.ignoreRouteCase?"start"===(this.getId()||"").toLowerCase()||"start"===(this.getRole()||"").toLowerCase():"start"===this.getId()||"start"===this.getRole()},n.nullObject=new e.Page,n.nullObject.children=c.observableArray([]),n.child=function(a){var d=this;return null==d._child&&(d._child={}),d._child[a]||(d._child[a]=c.computed(function(){var c=b.grep(this.children(),function(b){return b.id()===a})[0];return c||this.nullObject},this)),d._child[a]},e.getActivePage=function(){for(var a=e.page;null!=a.currentChildPage()();)a=a.currentChildPage()();return a},c.bindingHandlers.page={init:function(a,b,c,d,f){var g=null;return k.value(b())instanceof e.Page?(g=k.value(b()),g.element=a,null==g.allBindingsAccessor&&(g.allBindingsAccessor=c),null==g.viewModel&&(g.viewModel=d),null==g.bindingContext&&(g.bindingContext=f)):g=new e.Page(a,b,c,d,f),g.init()}};var s="page-href";e.useHTML5history=!1,e.rootURI="/",e.ignoreRouteCase=!1,e.dataAttribute=c.computed({read:function(){return s},write:function(a){if(!a){var b=new Error;throw b.message="Data Attribute cannot be blank",b}c.bindingHandlers[s]&&delete c.bindingHandlers[s],s=a,c.bindingHandlers[s]=u}}),e.Href=function(a,b,d,e,f){this.element=a,this.bindingContext=f,this.path=c.observable(),this.pageOrRelativePath=c.observable(b)};var t=e.Href.prototype;t.getParentPage=function(){return e.getParentPage(this.bindingContext)},t.init=function(){var a=this,b=a.getParentPage();a.path=c.computed(function(){var c=k.value(a.pageOrRelativePath()());return b.path(c)})},e.Href.hash="#",t.bind=function(){c.applyBindingsToNode(this.element,{attr:{href:this.path}})},t.update=function(a){this.pageOrRelativePath(a)},e.Href5=function(a,b,c,d,f){e.Href.apply(this,arguments)},e.Href5.prototype=new e.Href,e.Href5.history=a.History,e.Href5.prototype.bind=function(){var d=this;c.applyBindingsToNode(d.element,{attr:{href:d.path},click:function(c,f){var g=d.path();return(""===g||"/"===g)&&(g=b("base").attr("href")),f.shiftKey||f.ctrlKey||f.metaKey?void a.open(g,"_blank"):void e.Href5.history.pushState(null,null,g)}})};var u={init:function(a,b,c,d,f){var g=e.useHTML5history?e.Href5:e.Href,h=new g(a,b,c,d,f);h.init(),h.bind(),a.__ko__page=h},update:function(a,b){a.__ko__page.update(b)}};return c.bindingHandlers[e.dataAttribute()]=u,e.fx={},e.fx.cssAsync=function(a){return{showElement:function(c,d){var e=b(c.element);e.addClass(a),e.show();var f=setInterval(function(){clearInterval(f),e.addClass(a+"-in")},10),g=setInterval(function(){clearInterval(g),d&&d()},300)},hideElement:function(c,d){var e=b(c.element);if(c.pageHiddenOnce){e.removeClass(a+"-in");var f=setInterval(function(){clearInterval(f),d&&d(),e.hide()},300)}else c.pageHiddenOnce=!0,e.hide()}}},e.fx.zoom=e.fx.cssAsync("pagerjs-fx-zoom"),e.fx.flip=e.fx.cssAsync("pagerjs-fx-flip"),e.fx.popout=e.fx.cssAsync("pagerjs-fx-popout-modal"),e.fx.jQuerySync=function(a,c){return{showElement:function(c,d){a.call(b(c.element),300,d)},hideElement:function(a,d){c.call(b(a.element),300,function(){b(a.element).hide()}),d&&d()}}},e.fx.slide=e.fx.jQuerySync(b.fn.slideDown,b.fn.slideUp),e.fx.fade=e.fx.jQuerySync(b.fn.fadeIn,b.fn.fadeOut),e.startHistoryJs=function(b){var c="string"==typeof b?b:null;c&&e.Href5.history.pushState(null,null,c),e.Href5.history.Adapter.bind(a,"statechange",function(){var a=e.Href5.history.getState().url.replace(e.Href5.history.getBaseUrl(),"");i(a)}),e.Href5.history.Adapter.bind(a,"anchorchange",function(){var b=a.location.href.split("#")[1];i(b?"#"+b:"")}),b&&b.noGo||i(e.Href5.history.getState().url.replace(e.Href5.history.getBaseUrl(),""))},e.start=function(c){var d="string"==typeof c?c:null;d&&(a.location.hash=e.Href.hash+d);var f=function(){var b=a.location.href.split("#")[1];i(b?"#"+b:"")};b(a).bind("hashchange",f),c&&c.noGo||f()},e},c=a.define;"function"==typeof c&&"object"==typeof c.amd&&c.amd?c("pager",["knockout","jquery"],function(a,c){return b(c,a)}):a.pager=b($,ko)}(window);