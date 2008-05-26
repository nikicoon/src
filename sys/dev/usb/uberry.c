/*	$NetBSD: uberry.c,v 1.1 2008/05/26 00:23:05 christos Exp $	*/

/*-
 * Copyright (c) 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christos Zoulas.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific pberryr written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: uberry.c,v 1.1 2008/05/26 00:23:05 christos Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#if defined(__NetBSD__) || defined(__OpenBSD__)
#include <sys/device.h>
#include <sys/ioctl.h>
#elif defined(__FreeBSD__)
#include <sys/module.h>
#include <sys/bus.h>
#include <sys/ioccom.h>
#include <sys/conf.h>
#include <sys/fcntl.h>
#include <sys/filio.h>
#endif
#include <sys/conf.h>
#include <sys/file.h>
#include <sys/select.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/poll.h>

#include <dev/usb/usb.h>
#include <dev/usb/usbdi.h>
#include <dev/usb/usbdi_util.h>

#include <dev/usb/usbdevs.h>

#ifdef UBERRY_DEBUG
#define DPRINTF(x)	if (uberrydebug) logprintf x
#define DPRINTFN(n, x)	if (uberrydebug > n) logprintf x
int	uberrydebug = 0;
#else
#define DPRINTF(x)
#define DPRINTFN(n, x)
#endif

struct uberry_softc {
 	USBBASEDEVICE		sc_dev;
	usbd_device_handle	sc_udev;
};

static const struct usb_devno uberry_devs[] = {
	{ USB_VENDOR_RIM, USB_PRODUCT_RIM_BLACKBERRY },
	{ USB_VENDOR_RIM, USB_PRODUCT_RIM_BLACKBERRY_PEARL },
	{ USB_VENDOR_RIM, USB_PRODUCT_RIM_BLACKBERRY_PEARL_DUAL },
};

#define uberry_lookup(v, p) usb_lookup(uberry_devs, v, p)
#define UBERRY_CONFIG_NO 1

USB_DECLARE_DRIVER(uberry);

static void
uberry_cmd(struct uberry_softc *sc, uint8_t requestType, uint8_t reqno,
    uint8_t value, uint8_t index, void *data, uint8_t length)
{
	usb_device_request_t req;
	usbd_status err;
 
	DPRINTF(("berry cmd type=%x, number=%x, value=%d, index=%d, len=%d\n",
	    requestType, reqno, value, index, length));
        req.bmRequestType = requestType;
        req.bRequest = reqno;
        USETW(req.wValue, value); 
        USETW(req.wIndex, index);
        USETW(req.wLength, length);
   
        if ((err = usbd_do_request(sc->sc_udev, &req, data)) != 0)
		aprint_error_dev(sc->sc_dev, "sending command failed %d\n",
		    err);
}

static void
uberry_charge(struct uberry_softc *sc)
{
	char dummy[2];

#ifdef notyet
	if (sc->sc_udev->power == 250) {
		DPRINTF(("Power is already %d\n", sc->sc_udev->power));
		return;
	}
#endif

	uberry_cmd(sc, UT_READ | UT_VENDOR, 0xa5, 0, 1, dummy, 2);
	uberry_cmd(sc, UT_WRITE | UT_VENDOR, 0xa2, 0, 1, dummy, 0);
}

/*
 * Expose both the USB mass storage interface and the database access one
 */
static void
uberry_dual_mode(struct uberry_softc *sc)
{
	char dummy[2];

	uberry_cmd(sc, UT_READ | UT_VENDOR, 0xa9, 1, 1, dummy, 2);
}


USB_MATCH(uberry)
{
	USB_MATCH_START(uberry, uaa);

	DPRINTFN(50,("uberry_match\n"));

	return (uberry_lookup(uaa->vendor, uaa->product) != NULL ?
	    UMATCH_VENDOR_PRODUCT : UMATCH_NONE);
}

USB_ATTACH(uberry)
{
	USB_ATTACH_START(uberry, sc, uaa);
	usbd_device_handle	dev = uaa->device;
	char			*devinfop;
#ifdef notyet
	usbd_status		err;
#endif

	DPRINTFN(10,("uberry_attach: sc=%p\n", sc));

	sc->sc_dev = self;
	sc->sc_udev = dev;

	devinfop = usbd_devinfo_alloc(dev, 0);
	USB_ATTACH_SETUP;
	aprint_normal_dev(self, "%s\n", devinfop);
	usbd_devinfo_free(devinfop);

	uberry_charge(sc);
	if (uaa->product == USB_PRODUCT_RIM_BLACKBERRY_PEARL)
		uberry_dual_mode(sc);
#ifdef notyet
	err = usbd_set_config_no(dev, UBERRY_CONFIG_NO, 1);
	if (err) {
		aprint_error_dev(self, "setting config no failed\n");
		USB_ATTACH_ERROR_RETURN;
	}
#endif

	DPRINTFN(10, ("uberry_attach: %p\n", sc->sc_udev));

	usbd_add_drv_event(USB_EVENT_DRIVER_ATTACH, sc->sc_udev,
	   USBDEV(sc->sc_dev));

	USB_ATTACH_SUCCESS_RETURN;
}

USB_DETACH(uberry)
{
	USB_DETACH_START(uberry, sc);
#if defined(__NetBSD__) || defined(__OpenBSD__)
	DPRINTF(("uberry_detach: sc=%p flags=%d\n", sc, flags));
#elif defined(__FreeBSD__)
	DPRINTF(("uberry_detach: sc=%p\n", sc));
#endif

	usbd_add_drv_event(USB_EVENT_DRIVER_DETACH, sc->sc_udev,
	    USBDEV(sc->sc_dev));

	return (0);
}

#if defined(__NetBSD__) || defined(__OpenBSD__)
int
uberry_activate(device_ptr_t self, enum devact act)
{
	switch (act) {
	case DVACT_ACTIVATE:
		return (EOPNOTSUPP);
		break;

	case DVACT_DEACTIVATE:
		break;
	}
	return (0);
}
#endif

#if defined(__FreeBSD__)
DRIVER_MODULE(uberry, uhub, uberry_driver, uberry_devclass, usbd_driver_load, 0);
#endif /* defined(__FreeBSD__) */
