package main

import (
	"bufio"
	"github.com/warthog618/go-gpiocdev"
	"github.com/warthog618/go-gpiocdev/device/rpi"
	"log"
	"os"
	"time"
)

type team struct {
	name string
	pin  int
	line *gpiocdev.Line
}

func main() {

	chip, err := gpiocdev.NewChip("gpiochip0")
	if err != nil {
		log.Printf("chip error: %s", err)
		return
	}

	//offsets := []int{rpi.GPIO12, rpi.GPIO16, rpi.GPIO21, rpi.GPIO22}
	//l, err := gpiocdev.RequestLines(chip, offsets, gpiocdev.AsOutput(1, 1, 1, 1))
	//if err != nil {
	//	panic(err)
	//}
	//// revert lines to input on the way out.
	//defer func() {
	//	l.Reconfigure(gpiocdev.AsInput)
	//	fmt.Printf("Input pins %s:%v\n", chip, offsets)
	//	l.Close()
	//}()

	red := team{name: "red", pin: rpi.GPIO21}
	green := team{name: "green", pin: rpi.GPIO16}
	blue := team{name: "blue", pin: rpi.GPIO12}
	yellow := team{name: "yellow", pin: rpi.GPIO22}

	teams := []team{red, green, blue, yellow}

	// Ensure everything we touch gets reset to input
	defer func() {
		for i := range teams {
			if teams[i].line != nil {
				_ = teams[i].line.Reconfigure(gpiocdev.AsInput)
			}
		}
	}()

	for i := range teams {
		line, err := chip.RequestLine(teams[i].pin, gpiocdev.AsOutput(0))
		if err != nil {
			log.Printf("cannot request pin %d: %s", teams[i].pin, err)
			return
		}

		teams[i].line = line
	}

	keys := map[string]team{
		"1121638660": red,
		"1121704196": red,
		"1121835268": red,
		"1121769732": red,

		"1122162948": green,
		"1122097412": green,
		"1122031876": green,
		"1121966340": green,

		"1121442052": blue,
		"1121376516": blue,
		"1121310980": blue,
		"1121245444": blue,

		"1122490628": yellow,
		"1122425092": yellow,
		"1122359556": yellow,
		"1122228484": yellow,
	}

	log.Printf("Ready to scan\n\n")

	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {

		team, ok := keys[scanner.Text()]
		if ok {
			log.Printf("Key '%s' is in team '%s' (pin %d)\n", scanner.Text(), team.name, team.pin)
			team.line.SetValue(1)
			time.Sleep(time.Second)
			team.line.SetValue(0)
		} else {
			log.Printf("ERR: Key '%s' doesn't exist\n", scanner.Text())
		}

	}

	if scanner.Err() != nil {
		panic(scanner.Err())
	}
}
